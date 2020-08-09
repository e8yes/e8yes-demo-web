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

#ifndef CREATE_SCHEMA_H
#define CREATE_SCHEMA_H

#include <sqlite3.h>
#include <string>

namespace e8 {

static char const kNodeStateTableName[] = "node_states";
static char const kNodeStateTableNodeNameColumnName[] = "node_name";
static char const kNodeStateTableClockColumnName[] = "clock";
static char const kNodeStateTableSerializedDataColumnName[] = "serialized_data";

/**
 * @brief CreateAndOpenDatabase
 * @param file
 * @param override_data
 */
void CreateSchema(std::string const &file, bool override_data);

} // namespace e8

#endif // CREATE_SCHEMA_H
