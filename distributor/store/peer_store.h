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

#ifndef PEERSTORE_H
#define PEERSTORE_H

#include <map>
#include <mutex>
#include <string>

#include "distributor/store/entity.h"
#include "proto_cc/node.pb.h"

namespace e8 {

/**
 * @brief The PeerStore class Connects to the local persistent peer state storage and records the
 * local network topology.
 */
class PeerStore {
  public:
    explicit PeerStore(std::string const &file_path);

    /**
     * @brief AddPeer Adds a node to the peer set.
     *
     * Caution: this function should only be called by one process.
     * TODO: Use inter-process mutexes to increase fault tolerance even though this is not likely to
     * happen.
     *
     * @return true if the peer did not exist. Otherwise, false.
     */
    bool AddPeer(NodeState const &node);

    /**
     * @brief DeletePeer Deletes a node from the peer set.
     *
     * Caution: this function should only be called by one process.
     * TODO: Use inter-process mutexes to increase fault tolerance even though this is not likely to
     * happen.
     *
     * @return true if the peer did exist. Otherwise, false.
     */
    bool DeletePeer(std::string const &node_name);

    /**
     * @brief Peers Retrieves the peer set.
     */
    std::map<NodeName, NodeState> Peers();

  private:
    std::string const file_path_;
    std::mutex lock_;
};

} // namespace e8

#endif // PEERSTORE_H
