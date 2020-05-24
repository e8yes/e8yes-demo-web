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

#ifndef FILE_ACCESS_VALIDATOR_H
#define FILE_ACCESS_VALIDATOR_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_group_entity.h"
#include "demoweb_service/demoweb/proto_cc/file.pb.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

using FileAccessToken = std::vector<uint8_t>;

/**
 * @brief SignFileAccessToken Sign a token for a user allowing him to access the specified file
 * location.
 *
 * @param viewer Identity of the user who is allowed to use this token.
 * @param file_path File location this token is valid for.
 * @param access_mode Access mode of the location this token is valid for.
 * @param key_gen Key generator that holds the private signature key.
 * @return An access token for the specified location.
 */
FileAccessToken SignFileAccessToken(Identity const &viewer, std::string const &file_path,
                                    FileAccessMode access_mode, KeyGeneratorInterface *key_gen);

/**
 * @brief ValidateFileAccessToken Validate access to a location using a specified access mode
 * through a file access token for a specific user. If the validiation is succesful, it extracts the
 * file location that was signed into the token.
 *
 * @param viewer Identity of the viewer to validate against.
 * @param access_mode Access mode to the file location the user wants to use.
 * @param access_token The access token the viewer is holding.
 * @param key_gen Key generator that holds the public signature verification key.
 * @return Locate of the file the token permits access to if the token can be verified successfully.
 */
std::optional<std::string> ValidateFileAccessToken(Identity const &viewer,
                                                   FileAccessMode access_mode,
                                                   FileAccessToken const &access_token,
                                                   KeyGeneratorInterface *key_gen);

} // namespace e8

#endif // FILE_ACCESS_VALIDATOR_H
