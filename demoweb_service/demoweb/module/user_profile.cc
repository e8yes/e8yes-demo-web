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
#include <cstdint>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/file_metadata_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/constant/file_path.h"
#include "demoweb_service/demoweb/module/file_access_validator.h"
#include "demoweb_service/demoweb/module/file_metadata.h"
#include "demoweb_service/demoweb/module/file_util.h"
#include "demoweb_service/demoweb/module/retrieve_contact.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/proto_cc/file.pb.h"
#include "demoweb_service/demoweb/proto_cc/user_profile.pb.h"
#include "demoweb_service/demoweb/proto_cc/user_relation.pb.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace profile_internal {

std::string AllocateNewAvatarLocation(std::string const &user_id_str, FileFormat file_format,
                                      std::optional<std::string> const &old_path) {
    std::string avatar_dir_path =
        std::string("/") + kUserDataPathPrefix + "/" + user_id_str + "/" + kAvatarPathPrefix + "/";

    unsigned counter = 0;
    if (old_path.has_value()) {
        std::regex re("^" + avatar_dir_path + R"(([0-9]+)\.[a-zA-Z_]+$)");
        std::smatch sm;
        std::regex_match(old_path.value(), sm, re);
        assert(sm.size() == 2);

        counter = static_cast<unsigned>(std::stoi(sm[1].str()));
    }
    ++counter;

    std::optional<std::string> suffix = FileFormatSuffix(file_format);
    assert(suffix.has_value());
    return avatar_dir_path + std::to_string(counter) + "." + suffix.value();
}

UserPublicProfile BuildPublicProfile(UserEntity const &user, UserRelations const &relations,
                                     KeyGeneratorInterface *key_gen) {
    UserPublicProfile profile;
    profile.set_user_id(user.id.Value().value());
    assert(user.created_at.Value().has_value());
    profile.set_join_at(user.created_at.Value().value());
    *profile.mutable_relations() = {relations.begin(), relations.end()};

    if (user.alias.Value().has_value()) {
        profile.mutable_alias()->set_value(user.alias.Value().value());
    }

    if (user.avatar_path.Value().has_value()) {
        FileAccessToken avatar_path_token =
            SignFileAccessToken(user.id.Value().value(), user.avatar_path.Value().value(),
                                FileAccessMode::FAM_READ, key_gen);
        profile.mutable_avatar_readonly_access()->set_access_token(avatar_path_token);

        if (user.avatar_preview_path.Value().has_value()) {
            FileAccessToken avatar_preview_path_token = SignFileAccessToken(
                user.id.Value().value(), user.avatar_preview_path.Value().value(),
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

} // namespace profile_internal

bool UpdateProfile(std::optional<std::string> const &alias,
                   std::optional<std::string> const &biography, UserEntity *user,
                   ConnectionReservoirInterface *db_conns) {
    *user->alias.ValuePtr() = alias;
    *user->biography.ValuePtr() = biography;

    int num_rows_updated = Update(*user, TableNames::AUser(), /*override=*/true, db_conns);
    if (num_rows_updated == 0) {
        return false;
    }

    assert(num_rows_updated == 1);

    return true;
}

std::vector<UserPublicProfile> BuildPublicProfiles(std::optional<UserId> viewer_id,
                                                   std::vector<UserEntity> const &users,
                                                   KeyGeneratorInterface *key_gen,
                                                   ConnectionReservoirInterface *db_conns) {
    std::vector<UserId> target_user_ids;
    for (auto const &user : users) {
        target_user_ids.push_back(user.id.Value().value());
    }

    std::vector<UserPublicProfile> profiles;

    if (viewer_id.has_value()) {
        std::unordered_map<UserId, UserRelations> users_relations =
            GetUsersRelations(viewer_id.value(), target_user_ids, db_conns);

        for (auto const &user : users) {
            UserPublicProfile profile = profile_internal::BuildPublicProfile(
                user, users_relations[user.id.Value().value()], key_gen);
            profiles.push_back(profile);
        }
    } else {
        for (auto const &user : users) {
            UserPublicProfile profile =
                profile_internal::BuildPublicProfile(user, UserRelations(), key_gen);
            profiles.push_back(profile);
        }
    }

    return profiles;
}

AvatarSetup SetUpNewProfileAvatar(UserEntity const &user, FileFormat file_format,
                                  KeyGeneratorInterface *key_gen,
                                  ConnectionReservoirInterface *db_conns) {
    std::string location = profile_internal::AllocateNewAvatarLocation(
        user.id_str.Value().value(), file_format, user.avatar_path.Value());
    std::optional<FileMetadataEntity> avatar_file =
        AttachMetadataForFile(location, /*file_size=*/0, EncryptionSource::ESRC_NONE, db_conns);
    assert(avatar_file.has_value());
    assert(*avatar_file.value().path.Value() == location);

    // Assign the avatar file record to the user.
    UserEntity updated_user = user;
    *updated_user.avatar_path.ValuePtr() = location;
    uint64_t num_rows = Update(updated_user, TableNames::AUser(), /*replace=*/true, db_conns);
    assert(num_rows == 1);

    // Sign an access token.
    FileAccessToken access_token = SignFileAccessToken(user.id.Value().value(), location,
                                                       FileAccessMode::FAM_READWRITE, key_gen);

    AvatarSetup result;
    result.updated_user = updated_user;
    result.avatar_path_access_token = access_token;
    return result;
}

bool AcceptableProfileAvatarFileFormat(FileFormat file_format) {
    return file_format == FileFormat::FFMT_IMAGE_PNG ||
           file_format == FileFormat::FFMT_IMAGE_JPEG || file_format == FileFormat::FFMT_VIDEO_GIF;
}

} // namespace e8
