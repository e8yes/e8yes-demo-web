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

#ifndef NODESTATESTORE_H
#define NODESTATESTORE_H

#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "distributor/store/entity.h"
#include "proto_cc/node.pb.h"

namespace e8 {

using RevisionEpoch = int64_t;

/**
 * @brief The NodeStateStore class Connects to the local persistent node state storage. This allows
 * the updates to be shared by the processes using this class.
 */
class NodeStateStore {
  public:
    /**
     * @brief NodeStateReaderStore Initialize the database with the schema if it had not before.
     * This operation won't change the existing data.
     */
    NodeStateStore(std::string const &file_path);
    ~NodeStateStore();

    /**
     * @brief UpdateNodeStates Store the revision to the revision history and apply the revision to
     * create the latest snapshot if the former revision is avaliable. If future revisions are
     * avaiable, then it updates to the latest possible epoch. If the revision has been applied
     * before, it will do nothing.
     *
     * Caution: this function should only be called by one process.
     * TODO: Use inter-process mutexes to increase fault tolerance even though this is not likely to
     * happen.
     *
     * @return true if the revision has never been applied before, otherwise, false.
     */
    bool UpdateNodeStates(NodeStateRevision const &revision);

    /**
     * @brief Nodes Retrieves all nodes in the latest snapshot, with an optional to filter based
     * on node function and node status.
     */
    std::map<NodeName, NodeState> Nodes(std::optional<NodeFunction> const node_function,
                                        std::optional<NodeStatus> const node_status);

    /**
     * @brief CurrentRevisionEpoch Retrieve the current revision epoch applied to the latest node
     * state snapshot.
     */
    RevisionEpoch CurrentRevisionEpoch();

    /**
     * @brief Revisions Retrieves all the revisions bounded by the epoch interval [begin, end].
     * @return Returns all the available revisions within the interval.
     */
    std::vector<NodeStateRevision> Revisions(RevisionEpoch const begin, RevisionEpoch const end);

  private:
    std::string const file_path_;
    std::mutex lock_;
};

} // namespace e8

#endif // NODESTATESTORE_H
