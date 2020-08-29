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
#include <ctime>
#include <memory>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_group_entity.h"
#include "demoweb_service/demoweb/common_entity/user_group_has_file_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/file_access_validator.h"
#include "proto_cc/file.pb.h"
#include "proto_cc/identity.pb.h"
#include "keygen/key_generator_interface.h"
#include "keygen/sign_message.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

static char const kEncrypter[] = "FileAccessSigner";
static uint64_t const kFileSignatureValidDurationSecs = 60 * 10;

} // namespace

FileAccessToken SignFileAccessToken(UserId viewer_id, std::string const &file_path,
                                    FileAccessMode access_mode, KeyGeneratorInterface *key_gen) {
    SignableFileAccess file_access;
    file_access.set_viewer_id(viewer_id);
    file_access.set_file_path(file_path);
    file_access.set_access_mode(access_mode);

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    file_access.set_expiry_timestamp(cur_timestamp + kFileSignatureValidDurationSecs);

    std::string file_access_bytes;
    bool serialize_status = file_access.SerializeToString(&file_access_bytes);
    assert(serialize_status == true);

    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);

    return SignMessage(file_access_bytes, key_pair.key);
}

std::optional<std::string> ValidateFileAccessToken(UserId viewer_id, FileAccessMode access_mode,
                                                   FileAccessToken const &access_token,
                                                   KeyGeneratorInterface *key_gen) {
    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);
    assert(key_pair.public_key.has_value());

    std::optional<std::string> decoded_bytes =
        DecodeSignedMessage(access_token, key_pair.public_key.value());
    if (!decoded_bytes.has_value()) {
        return std::nullopt;
    }

    SignableFileAccess file_access;
    bool deserialize_status =
        file_access.ParseFromArray(decoded_bytes.value().data(), decoded_bytes.value().size());
    assert(deserialize_status == true);

    if (file_access.viewer_id() != viewer_id || file_access.access_mode() != access_mode) {
        return std::nullopt;
    }

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    if (cur_timestamp > file_access.expiry_timestamp()) {
        return std::nullopt;
    }

    return file_access.file_path();
}

void AddDirectFileAccessForUserGroup(std::string const &file_path,
                                     UserGroupEntity const &user_group, FileAccessMode access_mode,
                                     ConnectionReservoirInterface *db_conns) {
    UserGroupHasFileEntity file_group;
    *file_group.group_name.ValuePtr() = user_group.group_name.Value();
    *file_group.file_path.ValuePtr() = file_path;

    switch (access_mode) {
    case FAM_READ:
        *file_group.can_read.ValuePtr() = true;
        *file_group.can_write.ValuePtr() = false;
        break;
    case FAM_WRITE:
        *file_group.can_read.ValuePtr() = false;
        *file_group.can_write.ValuePtr() = true;
        break;
    case FAM_READWRITE:
        *file_group.can_read.ValuePtr() = true;
        *file_group.can_write.ValuePtr() = true;
        break;
    default:
        return;
    }

    uint64_t num_rows_affected = Update(file_group, TableNames::UserGroupHasFile(),
                                        /*override=*/true, db_conns);
    assert(num_rows_affected == 1);
}

bool ValidateDirectFileAccess(Identity const &viewer, std::string const &file_path,
                              FileAccessMode access_mode, ConnectionReservoirInterface *db_conns) {
    SqlQueryBuilder::Placeholder<SqlStrArr> viewer_participated_groups_ph;
    SqlQueryBuilder::Placeholder<SqlStr> file_path_ph;
    SqlQueryBuilder query;
    query.QueryPiece(TableNames::UserGroupHasFile())
        .QueryPiece(" WHERE file_path=")
        .Holder(&file_path_ph)
        .QueryPiece(" AND group_name=ANY(")
        .Holder(&viewer_participated_groups_ph)
        .QueryPiece(")");

    switch (access_mode) {
    case FAM_READ:
        query.QueryPiece(" AND can_read=TRUE");
        break;
    case FAM_WRITE:
        query.QueryPiece(" AND can_write=TRUE");
        break;
    case FAM_READWRITE:
        query.QueryPiece(" AND can_read=TRUE AND can_write=TRUE");
        break;
    default:
        return false;
    }

    query.SetValueToPlaceholder(viewer_participated_groups_ph,
                                std::make_shared<SqlStrArr>(std::vector<std::string>(
                                    {viewer.group_names().begin(), viewer.group_names().end()})));
    query.SetValueToPlaceholder(file_path_ph,
                                std::make_shared<SqlStr>(file_path, /*field_name=*/""));

    bool has_access = Exists(query, db_conns);
    return has_access;
}

} // namespace e8
