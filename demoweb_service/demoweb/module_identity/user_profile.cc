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
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_identity/user_profile.h"
#include "demoweb_service/demoweb/module_rbac/file_access_validator.h"
#include "demoweb_service/demoweb/proto_cc/user_profile.pb.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

bool UpdateProfile(std::optional<std::string> const &alias, UserEntity *user,
                   ConnectionReservoirInterface *db_conns) {
    if (alias.has_value()) {
        *user->alias.value_ptr() = alias.value();
    }

    int num_rows_updated = Update(*user, TableNames::AUser(), /*override=*/true, db_conns);
    if (num_rows_updated == 0) {
        return false;
    }

    assert(num_rows_updated == 1);

    return true;
}

UserPublicProfile BuildPublicProfile(UserEntity const &user, KeyGeneratorInterface *key_gen) {
    UserPublicProfile profile;
    profile.set_user_id(user.id.value().value());

    if (user.alias.value().has_value()) {
        profile.set_alias(user.alias.value().value());
    }

    if (user.avatar_path.value().has_value()) {
        FileAccessToken avatar_path_token =
            SignFileAccessToken(user.id.value().value(), user.avatar_path.value().value(),
                                FileAccessMode::FAM_READ, key_gen);
        profile.mutable_avatar_readonly_access()->set_access_token(avatar_path_token);

        if (user.avatar_preview_path.value().has_value()) {
            FileAccessToken avatar_preview_path_token = SignFileAccessToken(
                user.id.value().value(), user.avatar_preview_path.value().value(),
                FileAccessMode::FAM_READ, key_gen);
            profile.mutable_avatar_preview_readonly_access()->set_access_token(
                avatar_preview_path_token);
        } else {
            // Use the avatar path as the preview path when preview doesn't exist.
            profile.mutable_avatar_preview_readonly_access()->set_access_token(avatar_path_token);
        }
    }

    return profile;
}

} // namespace e8
