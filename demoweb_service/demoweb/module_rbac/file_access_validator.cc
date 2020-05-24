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
#include <ctime>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_group_entity.h"
#include "demoweb_service/demoweb/module_rbac/file_access_validator.h"
#include "demoweb_service/demoweb/proto_cc/file.pb.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "keygen/key_generator_interface.h"
#include "keygen/sign_message.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {
namespace {

static char const kEncrypter[] = "FileAccessSigner";
static uint64_t const kFileSignatureValidDurationSecs = 60 * 10;

} // namespace

FileAccessToken SignFileAccessToken(Identity const &viewer, std::string const &file_path,
                                    FileAccessMode access_mode, KeyGeneratorInterface *key_gen) {
    SignableFileAccess file_access;
    file_access.set_viewer_id(viewer.user_id());
    file_access.set_file_path(file_path);
    file_access.set_access_mode(access_mode);

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    file_access.set_expiry_timestamp(cur_timestamp + kFileSignatureValidDurationSecs);

    std::vector<uint8_t> file_access_bytes(file_access.ByteSize());
    bool serialize_status =
        file_access.SerializeToArray(file_access_bytes.data(), file_access_bytes.size());
    assert(serialize_status == true);

    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);

    return SignMessage(file_access_bytes, key_pair.key);
}

std::optional<std::string> ValidateFileAccessToken(Identity const &viewer,
                                                   FileAccessMode access_mode,
                                                   FileAccessToken const &access_token,
                                                   KeyGeneratorInterface *key_gen) {
    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);
    assert(key_pair.public_key.has_value());

    std::optional<std::vector<uint8_t>> decoded_bytes =
        DecodeSignedMessage(access_token, key_pair.public_key.value());
    if (!decoded_bytes.has_value()) {
        return std::nullopt;
    }

    SignableFileAccess file_access;
    bool deserialize_status =
        file_access.ParseFromArray(decoded_bytes.value().data(), decoded_bytes.value().size());
    assert(deserialize_status == true);

    if (file_access.viewer_id() != viewer.user_id() || file_access.access_mode() != access_mode) {
        return std::nullopt;
    }

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    if (cur_timestamp > file_access.expiry_timestamp()) {
        return std::nullopt;
    }

    return file_access.file_path();
}

} // namespace e8
