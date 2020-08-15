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
#include <sqlite3.h>
#include <string>

#include "distributor/store/node_state_schema.h"

namespace e8 {

void CreateNodeStateStoreSchema(std::string const &file, bool override_data) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file.c_str(), &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    if (override_data) {
        std::string drop_current_revision_epoch_table_stmt =
            std::string("DROP TABLE IF EXISTS ") + kCurrentRevisionEpochTableName;
        rc = sqlite3_exec(db, drop_current_revision_epoch_table_stmt.c_str(), /*callback=*/nullptr,
                          /*data=*/nullptr,
                          /*errormsg=*/nullptr);
        assert(rc == SQLITE_OK);

        std::string drop_revision_history_table_stmt =
            std::string("DROP TABLE IF EXISTS ") + kRevisionHistoryTableName;
        rc = sqlite3_exec(db, drop_revision_history_table_stmt.c_str(), /*callback=*/nullptr,
                          /*data=*/nullptr,
                          /*errormsg=*/nullptr);
        assert(rc == SQLITE_OK);

        std::string drop_node_states_table_stmt =
            std::string("DROP TABLE IF EXISTS ") + kNodeStateTableName;
        rc = sqlite3_exec(db, drop_node_states_table_stmt.c_str(), /*callback=*/nullptr,
                          /*data=*/nullptr,
                          /*errormsg=*/nullptr);
        assert(rc == SQLITE_OK);
    }

    std::string create_current_revision_epoch_table_stmt =
        std::string("CREATE TABLE IF NOT EXISTS ") + kCurrentRevisionEpochTableName + "(" +
        kCurrentRevisionEpochVersionNumberColumnName + " INTEGER NOT NULL)";
    rc = sqlite3_exec(db, create_current_revision_epoch_table_stmt.c_str(), /*callback=*/nullptr,
                      /*data=*/nullptr,
                      /*errormsg=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string create_revision_history_table_stmt =
        std::string("CREATE TABLE IF NOT EXISTS ") + kRevisionHistoryTableName + "(" +
        kRevisionHistoryTableRevisionEpochColumnName + " INTEGER NOT NULL," +
        kRevisionHistoryTableRevisionDataColumnName + " COLLATE BINARY NOT NULL," + "PRIMARY KEY(" +
        kRevisionHistoryTableRevisionEpochColumnName + "))";
    rc = sqlite3_exec(db, create_revision_history_table_stmt.c_str(), /*callback=*/nullptr,
                      /*data=*/nullptr,
                      /*errormsg=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string create_node_state_table_stmt =
        std::string("CREATE TABLE IF NOT EXISTS ") + kNodeStateTableName + "(" +
        kNodeStateTableNodeNameColumnName + " VARCHAR(32) NOT NULL," +
        kNodeStateTableNodeDataColumnName + " COLLATE BINARY NOT NULL," + "PRIMARY KEY(" +
        kNodeStateTableNodeNameColumnName + "))";
    rc = sqlite3_exec(db, create_node_state_table_stmt.c_str(), /*callback=*/nullptr,
                      /*data=*/nullptr,
                      /*errormsg=*/nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_close_v2(db);
    assert(rc == SQLITE_OK);
}

} // namespace e8
