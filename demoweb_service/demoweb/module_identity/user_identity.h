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

#ifndef USER_IDENTITY_H
#define USER_IDENTITY_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "keygen/key_generator_interface.h"

namespace e8 {

using SecurityKeyHash = std::string;
using SignedIdentity = std::vector<uint8_t>;

/**
 * @brief DigestSecurityKey A one-way encryption function that hashes a security key into a
 * cryptograhpic token.
 *
 * @param security_key The security key to be encrypted.
 * @return The hashed token if no error occurs, otherwise, a nullptr.
 */
std::optional<SecurityKeyHash> DigestSecurityKey(std::string const &security_key);

/**
 * @brief SignIdentity Validates the security key against the checksum then signs and encrypts
 * identity the into a token.
 *
 * @param user The user the security key to validate against.
 * @param security_key The security key used to authorize the signature of the identity.
 * @param key_gen Key generator that holds the private signature key.
 * @return The signed identity token if the security key validation is successful. Otherwise, it
 * returns a nullopt.
 */
std::optional<SignedIdentity> SignIdentity(UserEntity const &user, std::string const &security_key,
                                           KeyGeneratorInterface *key_gen);

/**
 * @brief ValidateSignedIdentity Decode the signed identity token into the identity object.
 *
 * @param signed_identity .
 * @param key_gen .
 * @return The identity object if the signed identity token can be sucessfully validated. Otherwise,
 * it returns a nullopt.
 */
std::optional<Identity> ValidateSignedIdentity(SignedIdentity const &signed_identity,
                                               KeyGeneratorInterface *key_gen);

} // namespace e8

#endif // USER_IDENTITY_H
