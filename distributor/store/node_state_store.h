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

#include <map>
#include <sqlite3.h>
#include <string>

#include "proto_cc/node.pb.h"

namespace e8 {

using NodeName = std::string;

/**
 * @brief The NodeStateStore class Connects and reads from the local persistent node state storage.
 * This allows the updates from the writer store to be shared by the processes using this class.
 */
class NodeStateReaderStore {
  public:
    /**
     * @brief NodeStateReaderStore Initialize the database with the schema if it had not before.
     * This operation won't change the existing data.
     */
    NodeStateReaderStore(std::string const &file_path);
    ~NodeStateReaderStore();

    /**
     * @brief Load Reads the current node states.
     */
    std::map<NodeName, NodeState> Load();

  private:
    std::string const file_path_;
};

/**
 * @brief The NodeStateWriterStore class Connects and writes to the local persistent node state
 * storage. This class updates the node states sharable with the reader stores.
 */
class NodeStateWriterStore {
  public:
    /**
     * @brief NodeStateWriterStore Initialize the database with the schema. If the schema exists and
     * override_data is set to true, then it will remove all the data before the initialization.
     */
    NodeStateWriterStore(std::string const &file_path, bool override_data);
    ~NodeStateWriterStore();

    /**
     * @brief Update Writes the new node states to the persistent store.
     */
    void Update(std::map<NodeName, NodeState> const &node_states);

  private:
    std::string const file_path_;
};

} // namespace e8

#endif // NODESTATESTORE_H
