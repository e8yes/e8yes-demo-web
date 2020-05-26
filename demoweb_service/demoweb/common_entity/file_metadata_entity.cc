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

#include "demoweb_service/demoweb/common_entity/file_metadata_entity.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"

namespace e8 {

FileMetadataEntity::FileMetadataEntity()
    : SqlEntityInterface({&path, &format, &encryption_key_source, &storage_size, &created_at,
                          &last_modified_at}) {}

FileMetadataEntity::FileMetadataEntity(FileMetadataEntity const &other) : FileMetadataEntity() {
    path = other.path;
    format = other.format;
    encryption_key_source = other.encryption_key_source;
    storage_size = other.storage_size;
    created_at = other.created_at;
    last_modified_at = other.last_modified_at;
}

FileMetadataEntity &FileMetadataEntity::operator=(FileMetadataEntity const &other) {
    path = other.path;
    format = other.format;
    encryption_key_source = other.encryption_key_source;
    storage_size = other.storage_size;
    created_at = other.created_at;
    last_modified_at = other.last_modified_at;
    return *this;
}

} // namespace e8
