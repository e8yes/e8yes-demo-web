/**
 * e8yes demo web.
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
#include "demoweb_service/demoweb/module/user_identity.h"
#include "identity/trustable_identity.h"
#include "keygen/key_generator_interface.h"
#include "proto_cc/identity.pb.h"

namespace e8 {
namespace {

static char const kDigestAlgorithmPrefix[] = "$2y$";
static unsigned const kDigestStrength = 11;
static unsigned const kNumRandomBytes = 16;
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
    assert(user.id.Value().has_value());
    assert(user.security_key_hash.Value().has_value());
    auto [settings, ground_truth_key_hash] =
        ParseSecurityKeyHash(user.security_key_hash.Value().value());

    crypt_data data;
    char *hash = crypt_r(security_key.c_str(), settings.c_str(), &data);
    assert(hash != nullptr);
    if (std::string(hash) != ground_truth_key_hash) {
        return std::nullopt;
    }

    Identity identity;
    identity.set_user_id(user.id.Value().value());
    *identity.mutable_group_names() = {user.group_names.Value().begin(),
                                       user.group_names.Value().end()};
    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    identity.set_expiry_timestamp(cur_timestamp + kIdentityValidDurationSecs);

    return SignIdentity(identity, key_gen);
}

} // namespace e8
