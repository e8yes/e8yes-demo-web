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
#include <map>
#include <sqlite3.h>
#include <string>
#include <utility>

#include "distributor/store/create_schema.h"
#include "distributor/store/node_state_store.h"

namespace e8 {
namespace {

void UpsertNodeState(std::string const &node_name, NodeState const &node_state,
                     sqlite3_stmt *stmt) {
    int rc = sqlite3_reset(stmt);
    assert(rc == SQLITE_OK);

    rc = sqlite3_bind_text(stmt, /*index=*/1, node_name.c_str(), node_name.size() + 1, nullptr);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int64(stmt, /*index=*/2, node_state.clock());
    assert(rc == SQLITE_OK);
    std::string serialized = node_state.SerializeAsString();
    rc = sqlite3_bind_blob(stmt, /*index=*/3, serialized.data(), serialized.size(), nullptr);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_text(stmt, /*index=*/4, node_name.c_str(), node_name.size() + 1, nullptr);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);
}

} // namespace

NodeStateReaderStore::NodeStateReaderStore(std::string const &file_path) : file_path_(file_path) {
    CreateSchema(file_path, /*override_data=*/false);
}

NodeStateReaderStore::~NodeStateReaderStore() {}

NodeStateWriterStore::NodeStateWriterStore(std::string const &file_path, bool override_data)
    : file_path_(file_path) {
    CreateSchema(file_path, override_data);
}

NodeStateWriterStore::~NodeStateWriterStore() {}

std::map<NodeName, NodeState> NodeStateReaderStore::Load() {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("SELECT ") + kNodeStateTableNodeNameColumnName + "," +
                      kNodeStateTableClockColumnName + "," +
                      kNodeStateTableSerializedDataColumnName + " FROM " + kNodeStateTableName;

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    std::map<NodeName, NodeState> result;
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        char const *node_name = reinterpret_cast<char const *>(sqlite3_column_text(stmt, 0));

        int64_t clock = sqlite3_column_int64(stmt, 1);

        void const *serialized_data = sqlite3_column_blob(stmt, 2);
        int serialized_data_bytes = sqlite3_column_bytes(stmt, 2);

        NodeState node_state;
        node_state.ParseFromArray(serialized_data, serialized_data_bytes);
        node_state.set_clock(clock);

        result.insert(std::make_pair(std::string(node_name), node_state));
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}

void NodeStateWriterStore::Update(std::map<NodeName, NodeState> const &node_states) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("INSERT INTO ") + kNodeStateTableName + "(" +
                      kNodeStateTableNodeNameColumnName + "," + kNodeStateTableClockColumnName +
                      "," + kNodeStateTableSerializedDataColumnName + ")VALUES(?,?,?)ON CONFLICT(" +
                      kNodeStateTableNodeNameColumnName + ")DO UPDATE SET " +
                      kNodeStateTableClockColumnName + "=excluded." +
                      kNodeStateTableClockColumnName + "," +
                      kNodeStateTableSerializedDataColumnName + "=excluded." +
                      kNodeStateTableSerializedDataColumnName + " WHERE " +
                      kNodeStateTableNodeNameColumnName + "=?";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    for (auto const &[node_name, node_state] : node_states) {
        UpsertNodeState(node_name, node_state, stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

} // namespace e8
