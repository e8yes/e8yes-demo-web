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

#ifndef PEER_SCHEMA_H
#define PEER_SCHEMA_H

#include <string>

namespace e8 {

static char const kPeerTableName[] = "peers";
static char const kPeerTableNodeNameColumnName[] = "node_name";
static char const kPeerTableNodeDataColumnName[] = "node_data";

/**
 * @brief CreatePeerStoreSchema Initializes the database with the peer schema.
 *
 * @param file File path to the sqlite file.
 * @param override_data Whether to re-create the schema.
 */
void CreatePeerStoreSchema(std::string const &file, bool override_data);

} // namespace e8

#endif // PEER_SCHEMA_H
