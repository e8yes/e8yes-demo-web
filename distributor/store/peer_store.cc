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

#include <map>
#include <mutex>
#include <sqlite3.h>
#include <string>

#include "distributor/store/entity.h"
#include "distributor/store/peer_schema.h"
#include "distributor/store/peer_store.h"
#include "proto_cc/node.pb.h"

namespace e8 {

PeerStore::PeerStore(std::string const &file_path) : file_path_(file_path) {
    CreatePeerStoreSchema(file_path, /*override_data=*/false);
}

bool PeerStore::AddPeer(NodeState const &node) {
    lock_.lock();

    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("INSERT INTO ") + kPeerTableName + "(" +
                      kPeerTableNodeNameColumnName + "," + kPeerTableNodeDataColumnName +
                      ")VALUES(?,?)ON CONFLICT(" + kPeerTableNodeNameColumnName + ")DO NOTHING";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_text(stmt, /*index=*/1, node.name().c_str(), node.name().size() + 1, nullptr);
    assert(rc == SQLITE_OK);
    std::string serialized = node.SerializeAsString();
    rc = sqlite3_bind_blob(stmt, /*index=*/2, serialized.data(), serialized.size(), nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);

    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);

    int num_rows_inserted = sqlite3_total_changes(db);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    lock_.unlock();

    return num_rows_inserted == 1;
}

bool PeerStore::DeletePeer(std::string const &node_name) {
    lock_.lock();

    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("DELETE FROM ") + kPeerTableName + " WHERE " +
                      kPeerTableNodeNameColumnName + "=?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_text(stmt, /*index=*/1, node_name.c_str(), node_name.size() + 1, nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);

    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);

    int num_rows_inserted = sqlite3_total_changes(db);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    lock_.unlock();

    return num_rows_inserted == 1;
}

std::map<NodeName, NodeState> PeerStore::Peers() {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("SELECT ") + kPeerTableNodeNameColumnName + "," +
                      kPeerTableNodeDataColumnName + " FROM " + kPeerTableName;
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    std::map<NodeName, NodeState> result;
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        char const *node_name = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 0));

        void const *serialized_data = sqlite3_column_blob(stmt, 1);
        int serialized_data_bytes = sqlite3_column_bytes(stmt, 1);

        NodeState node_state;
        node_state.ParseFromArray(serialized_data, serialized_data_bytes);

        result.insert(std::make_pair(std::string(node_name), node_state));
    }

    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    return result;
}

} // namespace e8
