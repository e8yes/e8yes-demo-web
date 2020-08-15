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

static char const kRevisionHistoryTableName[] = "revision_history";
static char const kRevisionHistoryTableRevisionEpochColumnName[] = "revision_epoch";
static char const kRevisionHistoryTableRevisionDataColumnName[] = "revision_data";

static char const kCurrentRevisionEpochTableName[] = "current_revision_epoch";
static char const kCurrentRevisionEpochVersionNumberColumnName[] = "version_number";

static char const kNodeStateTableName[] = "node_states";
static char const kNodeStateTableNodeNameColumnName[] = "node_name";
static char const kNodeStateTableNodeDataColumnName[] = "node_data";

/**
 * @brief CreateNodeStateStoreSchema Initializes the database with the node state schema.
 *
 * @param file File path to the sqlite file.
 * @param override_data Whether to re-create the schema.
 */
void CreateNodeStateStoreSchema(std::string const &file, bool override_data);

} // namespace e8

#endif // CREATE_SCHEMA_H
