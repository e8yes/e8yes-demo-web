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
#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_map>

#include "proto_cc/replication.pb.h"
#include "replication/raft/common_types.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/integration_test/key_value_store.h"

namespace e8 {
namespace {

constexpr char const *kKvStoreSnapshotFile = "replication_kv_store.snapshot";

} // namespace

ReplicationKvStore::ReplicationKvStore(RaftLogOffset preferred_snapshot_frequency)
    : preferred_snapshot_frequency_(preferred_snapshot_frequency) {}

ReplicationKvStore::~ReplicationKvStore() {}

std::string ReplicationKvStore::Run(std::string const &command) {
    ReplicationKvStoreCommand kv_command;
    kv_command.ParseFromString(command);

    ReplicationKvStoreReturnValue return_value;

    switch (kv_command.command_case()) {
    case ReplicationKvStoreCommand::CommandCase::kGet: {
        auto it = data_.kvs().find(kv_command.get().key());
        if (it == data_.kvs().end()) {
            return_value.set_has_key(false);
        } else {
            return_value.set_has_key(true);
            return_value.set_value(it->second);
        }
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::kPut: {
        (*data_.mutable_kvs())[kv_command.put().key()] = kv_command.put().value();
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::kPutAppend: {
        (*data_.mutable_kvs())[kv_command.put_append().key()] += kv_command.put_append().value();
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::COMMAND_NOT_SET: {
        assert(false);
    }
    }

    return return_value.SerializeAsString();
}

RaftLogOffset ReplicationKvStore::PreferredSnapshotFrequency() const {
    return preferred_snapshot_frequency_;
}

void ReplicationKvStore::Save() const {
    remove(kKvStoreSnapshotFile);

    std::fstream f;
    f.open(kKvStoreSnapshotFile, std::ios::out | std::ios::binary);
    assert(f.is_open());

    data_.SerializeToOstream(&f);

    f.close();
}

void ReplicationKvStore::Restore() {
    std::fstream f;
    f.open(kKvStoreSnapshotFile, std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        return;
    }

    data_.ParseFromIstream(&f);

    f.close();
}

void ReplicationKvStore::Reset() { data_.mutable_kvs()->clear(); }

} // namespace e8
