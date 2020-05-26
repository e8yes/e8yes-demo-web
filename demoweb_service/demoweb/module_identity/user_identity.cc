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
#include <crypt.h>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/module_identity/user_identity.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "keygen/key_generator_interface.h"
#include "keygen/sign_message.h"

namespace e8 {
namespace {

static char const kDigestAlgorithmPrefix[] = "$2y$";
static unsigned const kDigestStrength = 11;
static unsigned const kNumRandomBytes = 16;
static char const kEncrypter[] = "DemowebIdentitySigner";
static uint64_t const kIdentityValidDurationSecs = 60 * 10;

std::vector<std::string> Split(std::string const &input, char delimiter) {
    std::stringstream ss(input);
    std::string part;
    std::vector<std::string> parts;
    while (std::getline(ss, part, delimiter)) {
        parts.push_back(std::move(part));
    }
    return parts;
}

SecurityKeyHash FormatSecurityKeyHash(char const *settings, char const *hash) {
    return SecurityKeyHash(std::string(settings) + "_" + std::string(hash));
}

std::pair<std::string, std::string> ParseSecurityKeyHash(SecurityKeyHash const &key_hash) {
    std::vector<std::string> parts = Split(key_hash, '_');
    assert(parts.size() == 2);
    return std::make_pair(parts[0], parts[1]);
}

} // namespace

std::optional<SecurityKeyHash> DigestSecurityKey(std::string const &security_key) {
    // Collect salt.
    std::ifstream random_source("/dev/urandom");
    assert(random_source.is_open());

    char random_bytes[kNumRandomBytes];
    random_source.read(random_bytes, kNumRandomBytes);

    random_source.close();

    // Create settings for the crytographic algorithm.
    char settings[CRYPT_GENSALT_OUTPUT_SIZE];
    char *salt_result = crypt_gensalt_rn(kDigestAlgorithmPrefix, kDigestStrength, random_bytes,
                                         kNumRandomBytes, settings, CRYPT_GENSALT_OUTPUT_SIZE);
    assert(salt_result != nullptr);

    // Hash the security key.
    crypt_data data;
    char *hash = crypt_r(security_key.c_str(), settings, &data);
    if (hash == nullptr) {
        return std::nullopt;
    }

    // Format and output the token.
    return FormatSecurityKeyHash(settings, hash);
}

std::optional<SignedIdentity> SignIdentity(UserEntity const &user, std::string const &security_key,
                                           KeyGeneratorInterface *key_gen) {
    assert(user.id.value().has_value());
    assert(user.security_key_hash.value().has_value());
    auto [settings, ground_truth_key_hash] =
        ParseSecurityKeyHash(user.security_key_hash.value().value());

    crypt_data data;
    char *hash = crypt_r(security_key.c_str(), settings.c_str(), &data);
    assert(hash != nullptr);
    if (std::string(hash) != ground_truth_key_hash) {
        return std::nullopt;
    }

    Identity identity;
    identity.set_user_id(user.id.value().value());
    *identity.mutable_group_names() = {user.group_names.value().begin(),
                                       user.group_names.value().end()};
    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    identity.set_expiry_timestamp(cur_timestamp + kIdentityValidDurationSecs);

    std::string identity_bytes;
    bool serialize_status = identity.SerializeToString(&identity_bytes);
    assert(serialize_status == true);

    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);

    return SignMessage(identity_bytes, key_pair.key);
}

std::optional<Identity> ValidateSignedIdentity(SignedIdentity const &signed_identity,
                                               KeyGeneratorInterface *key_gen) {
    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);
    assert(key_pair.public_key.has_value());

    std::optional<std::string> decoded_bytes =
        DecodeSignedMessage(signed_identity, key_pair.public_key.value());
    if (!decoded_bytes.has_value()) {
        return std::nullopt;
    }

    Identity identity;
    bool deserialize_status =
        identity.ParseFromArray(decoded_bytes.value().data(), decoded_bytes.value().size());
    assert(deserialize_status == true);

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    if (cur_timestamp > identity.expiry_timestamp()) {
        return std::nullopt;
    }

    return identity;
}

} // namespace e8
