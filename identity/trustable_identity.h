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

#ifndef TRUSTABLE_IDENTITY_H
#define TRUSTABLE_IDENTITY_H

#include <optional>

#include "keygen/key_generator_interface.h"
#include "proto_cc/identity.pb.h"

namespace e8 {

using SignedIdentity = std::string;

/**
 * @brief SignIdentity Serializes then encrypts the identity into a signature token.
 *
 * @param identity The identity to be encrypted.
 * @param key_gen Key generator that holds the private signature key.
 * @return The signed identity token if the security key validation is successful. Otherwise, it
 * returns a nullopt.
 */
std::optional<SignedIdentity> SignIdentity(Identity const &identity,
                                           KeyGeneratorInterface *key_gen);

/**
 * @brief ValidateSignedIdentity Decode the signed identity token into the identity object.
 *
 * @param signed_identity The signature to be validate and extract information from.
 * @param key_gen Key generator that holds the public signature verification key..
 * @return The identity object if the signed identity token can be sucessfully validated. Otherwise,
 * it returns a nullopt.
 */
std::optional<Identity> ValidateSignedIdentity(SignedIdentity const &signed_identity,
                                               KeyGeneratorInterface *key_gen);

} // namespace e8

#endif // TRUSTABLE_IDENTITY_H
