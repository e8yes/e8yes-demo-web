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

#ifndef FILE_METADATA_H
#define FILE_METADATA_H

#include <cstdint>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/file_metadata_entity.h"
#include "proto_cc/encryption_source.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief AttachMetadataForFile Creates or updates metadata for a physical file. The physical file
 * may not have to exist. Metadata of the file is defined in the FileMetadataEntity.
 *
 * @param file_path Path to the file to attach metadata to.
 * @param encryption_source Who holds the key to encrypt/decrypt the file.
 * @param db_conns Connections to the DemoWeb database.
 * @return The saved file metadata if the attachment succeed.
 */
std::optional<FileMetadataEntity> AttachMetadataForFile(std::string const &file_path,
                                                        uint64_t file_size,
                                                        EncryptionSource encryption_source,
                                                        ConnectionReservoirInterface *db_conns);

} // namespace e8

#endif // FILE_METADATA_H
