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
#include <sqlite3.h>
#include <string>

#include "distributor/store/peer_schema.h"

namespace e8 {

void CreatePeerStoreSchema(std::string const &file, bool override_data) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file.c_str(), &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    if (override_data) {
        std::string drop_peers_table_stmt = std::string("DROP TABLE IF EXISTS ") + kPeerTableName;
        rc = sqlite3_exec(db, drop_peers_table_stmt.c_str(), /*callback=*/nullptr,
                          /*data=*/nullptr,
                          /*errormsg=*/nullptr);
        assert(rc == SQLITE_OK);
    }

    std::string create_peer_table_stmt =
        std::string("CREATE TABLE IF NOT EXISTS ") + kPeerTableName + "(" +
        kPeerTableNodeNameColumnName + " VARCHAR(32) NOT NULL," + kPeerTableNodeDataColumnName +
        " COLLATE BINARY NOT NULL," + "PRIMARY KEY(" + kPeerTableNodeNameColumnName + "))";
    rc = sqlite3_exec(db, create_peer_table_stmt.c_str(), /*callback=*/nullptr,
                      /*data=*/nullptr,
                      /*errormsg=*/nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_close_v2(db);
    assert(rc == SQLITE_OK);
}

} // namespace e8
