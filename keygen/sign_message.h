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

#ifndef SIGN_MESSAGE_H
#define SIGN_MESSAGE_H

#include <optional>
#include <string>

namespace e8 {

/**
 * @brief SignMessage Sign and encrypt a message in the form of a byte array into a new byte array
 * using RSA cryptography.
 *
 * @param message_bytes The message to be signed.
 * @param raw_private_key The private key used to sign the message.
 * @return The signed and encrypted message.
 */
std::string SignMessage(std::string const &message_bytes, std::string const &raw_private_key);

/**
 * @brief DecodeSignedMessage Verify and decode the signed message using an associated public key of
 * the private key that was used to encrypt the message. If the integrity of the signed message
 * fails to be verified, this function will return nullopt.
 *
 * @param signed_message_bytes The message to be decoded.
 * @param raw_public_key The public key asssociated with the private key that the message was
 * encrypted.
 * @return The decoded message if the signed message is able to be verified.
 */
std::optional<std::string> DecodeSignedMessage(std::string const &signed_message_bytes,
                                               std::string const &raw_public_key);

} // namespace e8

#endif // SIGN_MESSAGE_H
