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

#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/module/file_access_validator.h"
#include "proto_cc/file.pb.h"
#include "proto_cc/user_profile.pb.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief UpdateProfile Update a user's profile. See class UserPublicProfile for what parameters are
 * considered public profile. Note that the semantics of optional isn't that whether if function
 * updates the field but that it treats them as actual value to update to the database table. A
 * std::opt value will be updated as NULL to the field.
 *
 * @param user Table entity of the user whose profile needs to be updated. The content of this
 * entity will be updated with the specified parameters after the function call.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return If the user pointed to by the ID of the entity doesn't exist, it will return false.
 * Otherwise, it returns true.
 */
bool UpdateProfile(std::optional<std::string> const &alias,
                   std::optional<std::string> const &biography, UserEntity *user,
                   ConnectionReservoirInterface *db_conns);

/**
 * @brief BuildPublicProfiles Extract public profile info from raw database entities and generate
 * profile info relative to the viewer.
 *
 * @param viewer_id viewer of the public profile to build relative to. If this parameter isn't
 * provided, user relation will not be fetched.
 * @param users A list of user to extract public profile from.
 * @param key_gen Key generator for signing the avatar path.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return The extracted public profile.
 */
std::vector<UserPublicProfile> BuildPublicProfiles(std::optional<UserId> viewer_id,
                                                   std::vector<UserEntity> const &users,
                                                   KeyGeneratorInterface *key_gen,
                                                   ConnectionReservoirInterface *db_conns);

/**
 * @brief The AvatarSetup struct Contains the user entity updated with the new avatar path and a
 * read/write file path access token for the newly assigned avatar path.
 */
struct AvatarSetup {
    UserEntity updated_user;
    FileAccessToken avatar_path_access_token;
};

/**
 * @brief SetUpNewProfileAvatar Set up a systematically uniformed file path for user avatar and
 * update the avatar path to the specified user.
 *
 * @param user User to set avatar for.
 * @param file_format The format of the new avatar file.
 * @param key_gen Key generator for signing a new avatar path.
 * @param db_conns Connections to the DemoWeb DB server.
 * @return See the above.
 */
AvatarSetup SetUpNewProfileAvatar(UserEntity const &user, FileFormat file_format,
                                  KeyGeneratorInterface *key_gen,
                                  ConnectionReservoirInterface *db_conns);

/**
 * @brief AcceptableAvatarFileFormat Determine if the supplied file format is acceptable as a
 * profile avatar.
 */
bool AcceptableProfileAvatarFileFormat(FileFormat file_format);

namespace profile_internal {

std::string AllocateNewAvatarLocation(std::string const &user_id_str, FileFormat file_format,
                                      std::optional<std::string> const &old_path);

} // namespace profile_internal
} // namespace e8

#endif // USER_PROFILE_H
