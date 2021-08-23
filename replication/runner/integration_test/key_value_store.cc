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
#include <string>
#include <unordered_map>

#include "proto_cc/replication.pb.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/integration_test/key_value_store.h"

namespace e8 {

ReplicationKvStore::ReplicationKvStore() {}

ReplicationKvStore::~ReplicationKvStore() {}

std::string ReplicationKvStore::Run(std::string const &command) {
    ReplicationKvStoreCommand kv_command;
    kv_command.ParseFromString(command);

    ReplicationKvStoreReturnValue return_value;

    switch (kv_command.command_case()) {
    case ReplicationKvStoreCommand::CommandCase::kGet: {
        auto it = kvs_.find(kv_command.get().key());
        if (it == kvs_.end()) {
            return_value.set_has_key(false);
        } else {
            return_value.set_has_key(true);
            return_value.set_value(it->second);
        }
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::kPut: {
        kvs_[kv_command.put().key()] = kv_command.put().value();
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::kPutAppend: {
        kvs_[kv_command.put_append().key()] += kv_command.put_append().value();
        break;
    }
    case ReplicationKvStoreCommand::CommandCase::COMMAND_NOT_SET: {
        assert(false);
    }
    }

    return return_value.SerializeAsString();
}

void ReplicationKvStore::Reset() { kvs_.clear(); }

} // namespace e8
