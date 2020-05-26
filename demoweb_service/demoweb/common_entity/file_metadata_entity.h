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

#ifndef FILEMETADATAENTITY_H
#define FILEMETADATAENTITY_H

#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

/**
 * @brief The FileMetadataEntity class C++ class representation of the database table
 * "file_metadata".
 */
class FileMetadataEntity : public SqlEntityInterface {
  public:
    FileMetadataEntity();
    FileMetadataEntity(FileMetadataEntity const &other);
    ~FileMetadataEntity() = default;

    FileMetadataEntity &operator=(FileMetadataEntity const &other);

    SqlStr path = SqlStr("path");
    SqlInt format = SqlInt("format");
    SqlInt encryption_key_source = SqlInt("encryption_key_source");
    SqlLong storage_size = SqlLong("storage_size");
    SqlTimestamp created_at = SqlTimestamp("created_at");
    SqlTimestamp last_modified_at = SqlTimestamp("last_modified_at");
};

} // namespace e8

#endif // FILEMETADATAENTITY_H
