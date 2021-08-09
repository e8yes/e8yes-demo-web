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

#ifndef PERSISTER_H
#define PERSISTER_H

#include <fstream>
#include <google/protobuf/repeated_field.h>
#include <mutex>
#include <string>
#include <utility>

#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"

namespace e8 {

/**
 * @brief The RaftPersister class Manages the saving and recovery of the persistent states. It also
 * acts as a mutually exclusive device. The caller must use std::lock_guard on this object before
 * using any of the member functions.
 */
class RaftPersister {
  public:
    /**
     * @brief RaftPersister On constuction of this object, it will attempt to recover the persistent
     * states from the specified file, if one exists, otherwise, the term value will be initialized
     * to zero while the role value will be initialized to RAFT_FOLLOWER.
     */
    RaftPersister(std::string const &file_path);
    ~RaftPersister();

    /**
     * @brief lock Mutually exclusively acquires this device.
     */
    void lock();

    /**
     * @brief unlock Releases this device.
     */
    void unlock();

    /**
     * @brief TermAndRole Reads the term and role states.
     */
    std::pair<RaftTerm, RaftRole> TermAndRole();

    /**
     * @brief SetTermAndRole Updates the term and role states.
     */
    void SetTermAndRole(std::pair<RaftTerm, RaftRole> const &term_and_role);

    /**
     * @brief LogEntriesRef Gets a pointer reference to the log entries state.
     */
    google::protobuf::RepeatedPtrField<LogEntry> *LogEntriesRef();

    /**
     * @brief Persist Saves the current states to the file.
     */
    void Persist();

  private:
    void Recover(std::string const &file_path);

    std::mutex lock_;
    std::fstream file_;
    RaftPersistentStates states_;
};

} // namespace e8

#endif // PERSISTER_H
