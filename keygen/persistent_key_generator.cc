/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cstdint>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "common/container/lru_hash_map.h"
#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

struct KeyUser {
    KeyUser(std::string const &encrypter, e8::PersistentKeyGenerator::KeyType key_type)
        : encrypter(encrypter), key_type(key_type) {}
    KeyUser(KeyUser const &) = default;

    bool operator==(KeyUser const &rhs) const {
        return encrypter == rhs.encrypter && key_type == rhs.key_type;
    }

    std::string encrypter;
    e8::PersistentKeyGenerator::KeyType key_type;
    int32_t padding;
};

namespace std {

template <> struct hash<KeyUser> {
    size_t operator()(KeyUser const &key_user) const {
        std::size_t h1 = std::hash<std::string>{}(key_user.encrypter);
        std::size_t h2 = std::hash<int>{}(key_user.key_type);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

namespace e8 {
namespace {

static unsigned const kNumCachedKeys = 10000;
static char const kKeyPersistenceTableName[] = "key_persistence";

class KeyPersistenceEntity : public SqlEntityInterface {
  public:
    KeyPersistenceEntity()
        : SqlEntityInterface({&encrypter, &key_type, &crypto_key, &crypto_public_key}) {}

    KeyPersistenceEntity(KeyPersistenceEntity const &other) : KeyPersistenceEntity() {
        encrypter = other.encrypter;
        key_type = other.key_type;
        crypto_key = other.crypto_key;
        crypto_public_key = other.crypto_public_key;
    }

    SqlStr encrypter = SqlStr("encrypter");
    SqlInt key_type = SqlInt("key_type");
    SqlStr crypto_key = SqlStr("crypto_key");
    SqlStr crypto_public_key = SqlStr("crypto_public_key");
};

class OnFetch {
  public:
    OnFetch(ConnectionReservoirInterface *reservoir) : reservoir_(reservoir) {}
    OnFetch(OnFetch const &) = default;
    ~OnFetch() = default;

    std::optional<KeyGeneratorInterface::Key> operator()(KeyUser const &key_user) {
        SqlQueryBuilder query;
        SqlQueryBuilder::Placeholder<SqlStr> encrypter;
        SqlQueryBuilder::Placeholder<SqlInt> key_type;
        query.query_piece(kKeyPersistenceTableName)
            .query_piece(" kp WHERE kp.encrypter=")
            .placeholder(&encrypter)
            .query_piece("AND kp.key_type=")
            .placeholder(&key_type);

        SqlStr encrypter_value = SqlStr(key_user.encrypter, "");
        SqlInt key_type_value = SqlInt(key_user.key_type);
        query.set_value_to_placeholder(encrypter, &encrypter_value);
        query.set_value_to_placeholder(key_type, &key_type_value);

        std::vector<std::tuple<KeyPersistenceEntity>> key_persistence =
            Query<KeyPersistenceEntity>(query, {"kp"}, reservoir_);

        if (key_persistence.empty()) {
            return std::nullopt;
        }

        KeyPersistenceEntity const &entity = std::get<0>(key_persistence[0]);
        KeyGeneratorInterface::Key key;
        assert(entity.crypto_key.value().has_value());
        key.key = entity.crypto_key.value().value();
        key.public_key = entity.crypto_public_key.value();

        return std::optional<KeyGeneratorInterface::Key>(key);
    }

  private:
    ConnectionReservoirInterface *reservoir_;
};

class OnEvict {
  public:
    OnEvict() {}
    OnEvict(OnEvict const &) = default;
    ~OnEvict() = default;

    void operator()(KeyGeneratorInterface::Key const &) {}
};

} // namespace

class PersistentKeyGenerator::PersistentKeyGeneratorImpl {
  public:
    PersistentKeyGeneratorImpl(ConnectionReservoirInterface *reservoir)
        : crypto_key_cache(kNumCachedKeys, OnFetch(reservoir), OnEvict()), reservoir_(reservoir) {}

    Key GenerateKey(KeyUser const &key_user);

  public:
    LruHashMap<KeyUser, Key, OnFetch, OnEvict> crypto_key_cache;

  private:
    Key GenerateAlphaNumeric(unsigned length);
    Key GenerateRSA(unsigned bit_len);

    ConnectionReservoirInterface *reservoir_;
    CryptoPP::AutoSeededRandomPool rng_;
};

PersistentKeyGenerator::Key
PersistentKeyGenerator::PersistentKeyGeneratorImpl::GenerateAlphaNumeric(unsigned bit_len) {
    CryptoPP::RSA::PrivateKey private_key;
    private_key.GenerateRandomWithKeySize(rng_, bit_len);

    PersistentKeyGenerator::Key result;
    CryptoPP::Base64Encoder sink(new CryptoPP::StringSink(result.key));
    private_key.DEREncode(sink);

    return result;
}

PersistentKeyGenerator::Key
PersistentKeyGenerator::PersistentKeyGeneratorImpl::GenerateRSA(unsigned bit_len) {
    CryptoPP::RSA::PrivateKey private_key;
    private_key.GenerateRandomWithKeySize(rng_, bit_len);

    CryptoPP::RSA::PublicKey public_key(private_key);

    PersistentKeyGenerator::Key result;
    CryptoPP::Base64Encoder private_key_sink(new CryptoPP::StringSink(result.key));
    private_key.DEREncode(private_key_sink);

    std::string public_key_str;
    CryptoPP::Base64Encoder public_key_sink(new CryptoPP::StringSink(public_key_str));
    public_key.DEREncodePublicKey(public_key_sink);
    result.public_key = public_key_str;

    return result;
}

PersistentKeyGenerator::Key
PersistentKeyGenerator::PersistentKeyGeneratorImpl::GenerateKey(KeyUser const &key_user) {
    Key key;

    switch (key_user.key_type) {
    case RANDOM_512_BITS:
        key = GenerateAlphaNumeric(/*bit_len=*/512);
        break;
    case RSA_4096_BITS:
        key = GenerateRSA(/*bit_len=*/4096);
        break;
    }

    KeyPersistenceEntity key_persistence;
    *key_persistence.encrypter.value_ptr() = key_user.encrypter;
    *key_persistence.key_type.value_ptr() = key_user.key_type;
    *key_persistence.crypto_key.value_ptr() = key.key;
    if (key.public_key.has_value()) {
        *key_persistence.crypto_public_key.value_ptr() = key.public_key.value();
    }

    uint64_t num_rows_updated =
        Update(key_persistence, kKeyPersistenceTableName, /*replace=*/false, reservoir_);
    if (num_rows_updated == 1) {
        // Key added successfully.
        return key;
    }

    // Key user existed before adding into the persistent store.
    std::optional<Key> fetched = crypto_key_cache.Fetch(key_user);
    assert(fetched.has_value());

    return fetched.value();
}

PersistentKeyGenerator::PersistentKeyGenerator(ConnectionReservoirInterface *reservoir)
    : impl(std::make_unique<PersistentKeyGeneratorImpl>(reservoir)) {}

PersistentKeyGenerator::~PersistentKeyGenerator() {}

PersistentKeyGenerator::Key PersistentKeyGenerator::KeyOf(std::string const &encrypter,
                                                          KeyType key_type) {
    KeyUser key_user(encrypter, key_type);
    std::optional<Key> key = impl->crypto_key_cache.Fetch(key_user);
    if (!key.has_value()) {
        key = impl->GenerateKey(key_user);
    }
    return key.value();
}

} // namespace e8
