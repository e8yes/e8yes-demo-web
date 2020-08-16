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
#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <sqlite3.h>
#include <string>
#include <utility>

#include "distributor/store/entity.h"
#include "distributor/store/node_state_schema.h"
#include "distributor/store/node_state_store.h"
#include "proto_cc/delta.pb.h"
#include "proto_cc/node.pb.h"

namespace e8 {
namespace {

static RevisionEpoch const kStartingRevisionEpoch = 1;

bool ExistRevision(RevisionEpoch const epoch, sqlite3 *db) {
    std::string sql = std::string("SELECT 1 FROM ") + kRevisionHistoryTableName + " WHERE " +
                      kRevisionHistoryTableRevisionEpochColumnName + "=?";

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    sqlite3_bind_int64(stmt, /*index=*/1, epoch);

    bool result = SQLITE_ROW == sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    return result;
}

sqlite3_stmt *UpsertNodeStateStatement(sqlite3 *db) {
    std::string sql = std::string("INSERT INTO ") + kNodeStateTableName + "(" +
                      kNodeStateTableNodeNameColumnName + "," + kNodeStateTableNodeDataColumnName +
                      ")VALUES(?,?)ON CONFLICT(" + kNodeStateTableNodeNameColumnName +
                      ")DO UPDATE SET " + kNodeStateTableNodeDataColumnName + "=excluded." +
                      kNodeStateTableNodeDataColumnName;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);
    return stmt;
}

void UpsertNodeState(std::string const &node_name, NodeState const &node_state,
                     sqlite3_stmt *stmt) {
    int rc = sqlite3_reset(stmt);
    assert(rc == SQLITE_OK);

    rc = sqlite3_bind_text(stmt, /*index=*/1, node_name.c_str(), node_name.size() + 1, nullptr);
    assert(rc == SQLITE_OK);
    std::string serialized = node_state.SerializeAsString();
    rc = sqlite3_bind_blob(stmt, /*index=*/2, serialized.data(), serialized.size(), nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);
}

sqlite3_stmt *DeleteNodeStateStatement(sqlite3 *db) {
    std::string sql = std::string("DELETE FROM ") + kNodeStateTableName + " WHERE " +
                      kNodeStateTableNodeNameColumnName + "=?";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);
    return stmt;
}

void DeleteNodeState(std::string const &node_name, sqlite3_stmt *stmt) {
    int rc = sqlite3_reset(stmt);
    assert(rc == SQLITE_OK);

    rc = sqlite3_bind_text(stmt, /*index=*/1, node_name.c_str(), node_name.size() + 1, nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);
}

std::vector<NodeStateRevision> LoadRevisionHistory(RevisionEpoch begin, RevisionEpoch end,
                                                   sqlite3 *db) {
    std::string sql = std::string("SELECT ") + kRevisionHistoryTableRevisionDataColumnName +
                      " FROM " + kRevisionHistoryTableName + " WHERE " +
                      kRevisionHistoryTableRevisionEpochColumnName + ">=? AND " +
                      kRevisionHistoryTableRevisionEpochColumnName + "<=?";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int(stmt, /*index=*/1, begin);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int(stmt, /*index=*/2, end);
    assert(rc == SQLITE_OK);

    std::vector<NodeStateRevision> revisions;
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        void const *serialized_revision = sqlite3_column_blob(stmt, 0);
        int serialized_revision_bytes = sqlite3_column_bytes(stmt, 0);

        NodeStateRevision revision;
        revision.ParseFromArray(serialized_revision, serialized_revision_bytes);

        revisions.push_back(revision);
    }

    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);

    return revisions;
}

std::optional<NodeStateRevision> LoadRevisionHistory(RevisionEpoch epoch, sqlite3 *db) {
    std::vector<NodeStateRevision> revisions =
        LoadRevisionHistory(/*begin=*/epoch, /*end=*/epoch, db);
    if (revisions.empty()) {
        return std::nullopt;
    } else {
        assert(revisions.size() == 1);
        return revisions[0];
    }
}

void WriteRevisionHistory(NodeStateRevision const &revision, sqlite3 *db) {
    std::string sql = std::string("INSERT INTO ") + kRevisionHistoryTableName + "(" +
                      kRevisionHistoryTableRevisionEpochColumnName + "," +
                      kRevisionHistoryTableRevisionDataColumnName + ")VALUES(?,?)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_bind_int64(stmt, /*index=*/1, revision.revision_epoch());
    assert(rc == SQLITE_OK);
    std::string serialized = revision.SerializeAsString();
    rc = sqlite3_bind_blob(stmt, /*index=*/2, serialized.data(), serialized.size(), nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);
    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);
}

RevisionEpoch GetCurrentRevisionEpoch(sqlite3 *db) {
    std::string sql = std::string("SELECT ") + kCurrentRevisionEpochVersionNumberColumnName +
                      " FROM " + kCurrentRevisionEpochTableName + " WHERE " +
                      kCurrentRevisionEpochIdColumnName + "=1";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    int state = sqlite3_step(stmt);
    if (SQLITE_ROW != state) {
        rc = sqlite3_finalize(stmt);
        assert(rc == SQLITE_OK);
        return kStartingRevisionEpoch - 1;
    }

    RevisionEpoch epoch = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);

    return epoch;
}

void SetCurrentRevisionEpoch(RevisionEpoch const epoch, sqlite3 *db) {
    std::string sql = std::string("INSERT INTO ") + kCurrentRevisionEpochTableName + "(" +
                      kCurrentRevisionEpochIdColumnName + "," +
                      kCurrentRevisionEpochVersionNumberColumnName + ")VALUES(1,?)ON CONFLICT(" +
                      kCurrentRevisionEpochIdColumnName + ")DO UPDATE SET " +
                      kCurrentRevisionEpochVersionNumberColumnName + "=excluded." +
                      kCurrentRevisionEpochVersionNumberColumnName;

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size() + 1, &stmt, /*pzTail=*/nullptr);
    assert(rc == SQLITE_OK);

    rc = sqlite3_bind_int64(stmt, /*index=*/1, epoch);
    assert(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    assert(rc == SQLITE_DONE);
    rc = sqlite3_finalize(stmt);
    assert(rc == SQLITE_OK);
}

void ApplyRevisionToCurrentNodeStates(NodeStateRevision const &revision, sqlite3 *db) {
    sqlite3_stmt *upsert_node_state_stmt = UpsertNodeStateStatement(db);
    sqlite3_stmt *delete_node_state_stmt = DeleteNodeStateStatement(db);

    for (auto const &[node_name, delta_operation] : revision.delta_operations()) {
        switch (delta_operation) {
        case DOP_ADD:
        case DOP_SWAP: {
            NodeState const &node_state = revision.nodes().at(node_name);
            UpsertNodeState(node_name, node_state, upsert_node_state_stmt);
            break;
        }
        case DOP_DELETE: {
            DeleteNodeState(node_name, delete_node_state_stmt);
            break;
        }
        default: {
            assert(false);
        }
        }
    }

    sqlite3_finalize(upsert_node_state_stmt);
    sqlite3_finalize(delete_node_state_stmt);
}

bool ContainsFunction(NodeState const &node_state, NodeFunction const node_function) {
    auto const &functions = node_state.functions();
    auto it = std::find(functions.begin(), functions.end(), node_function);
    return it != functions.end();
}

} // namespace

NodeStateStore::NodeStateStore(std::string const &file_path) : file_path_(file_path) {
    CreateNodeStateStoreSchema(file_path, /*override_data=*/false);
}

NodeStateStore::~NodeStateStore() {}

std::map<NodeName, NodeState> NodeStateStore::Nodes(std::optional<NodeFunction> const node_function,
                                                    std::optional<NodeStatus> const node_status) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::string sql = std::string("SELECT ") + kNodeStateTableNodeNameColumnName + "," +
                      kNodeStateTableNodeDataColumnName + " FROM " + kNodeStateTableName;

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

        if (node_function.has_value() && !ContainsFunction(node_state, *node_function)) {
            continue;
        }
        if (node_status.has_value() && node_state.status() != *node_status) {
            continue;
        }

        result.insert(std::make_pair(std::string(node_name), node_state));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}

RevisionEpoch NodeStateStore::CurrentRevisionEpoch() {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    RevisionEpoch epoch = GetCurrentRevisionEpoch(db);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    return epoch;
}

bool NodeStateStore::UpdateNodeStates(NodeStateRevision const &revision) {
    lock_.lock();

    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    if (ExistRevision(revision.revision_epoch(), db)) {
        lock_.unlock();
        return false;
    }
    WriteRevisionHistory(revision, db);

    // Update the node state snapshot if possible.
    RevisionEpoch current_revision = this->CurrentRevisionEpoch();
    std::optional<NodeStateRevision> next_revision;

    // Can only apply revision when the next adjacent revision exists.
    while ((next_revision = LoadRevisionHistory(current_revision + 1, db)).has_value()) {
        ApplyRevisionToCurrentNodeStates(*next_revision, db);
        ++current_revision;
    }

    // Move the revision epoch to that represents the current node state snapshot.
    SetCurrentRevisionEpoch(current_revision, db);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    lock_.unlock();

    return true;
}

std::vector<NodeStateRevision> NodeStateStore::Revisions(RevisionEpoch const begin,
                                                         RevisionEpoch const end) {
    sqlite3 *db;
    int rc = sqlite3_open_v2(file_path_.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
                             /*zVfs=*/nullptr);
    assert(rc == SQLITE_OK);

    std::vector<NodeStateRevision> revisions = LoadRevisionHistory(begin, end, db);

    rc = sqlite3_close(db);
    assert(rc == SQLITE_OK);

    return revisions;
}

} // namespace e8
