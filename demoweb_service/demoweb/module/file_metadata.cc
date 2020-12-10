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

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/file_metadata_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/file_metadata.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/encryption_source.pb.h"
#include "proto_cc/file.pb.h"

namespace e8 {
namespace {

std::optional<FileFormat> DetectFileFormat(std::string const &file_path) {
    std::string path;
    std::transform(file_path.begin(), file_path.end(), path.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::string::size_type pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        return std::nullopt;
    }

    std::string suffix = path.substr(pos);
    if (suffix == "jpeg" || suffix == "jpg") {
        return FileFormat::FFMT_IMAGE_JPEG;
    } else if (path == "png") {
        return FileFormat::FFMT_IMAGE_PNG;
    } else if (path == "mp4") {
        return FileFormat::FFMT_VIDEO_MPEG4;
    } else if (path == "gif") {
        return FileFormat::FFMT_VIDEO_GIF;
    } else if (path == "ogv") {
        return FileFormat::FFMT_VIDEO_OGV;
    } else if (path == "mp3") {
        return FileFormat::FFMT_AUDIO_MP3;
    } else if (path == "ogg") {
        return FileFormat::FFMT_AUDIO_OGG;
    } else {
        return std::nullopt;
    }
}

} // namespace

std::optional<FileMetadataEntity> AttachMetadataForFile(std::string const &file_path,
                                                        uint64_t file_size,
                                                        EncryptionSource encryption_source,
                                                        ConnectionReservoirInterface *db_conns) {
    FileMetadataEntity file;
    *file.path.ValuePtr() = file_path;
    *file.storage_size.ValuePtr() = file_size;

    std::optional<FileFormat> format = DetectFileFormat(file_path);
    if (!format.has_value()) {
        return std::nullopt;
    }
    *file.format.ValuePtr() = format.value();
    *file.encryption_key_source.ValuePtr() = encryption_source;

    TimestampMicros timestamp = CurrentTimestampMicros();
    *file.created_at.ValuePtr() = timestamp;
    *file.last_modified_at.ValuePtr() = timestamp;

    uint64_t num_rows_affected =
        Update(file, TableNames::FileMetadata(), /*replace=*/true, db_conns);
    assert(num_rows_affected == 1);

    return file;
}

} // namespace e8
