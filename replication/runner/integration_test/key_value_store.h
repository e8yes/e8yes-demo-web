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

#ifndef REPLICATION_KEY_VALUE_STORE_H
#define REPLICATION_KEY_VALUE_STORE_H

#include <string>
#include <unordered_map>

#include "replication/runner/command_queue.h"

namespace e8 {

/**
 * @brief The ReplicationKvStore class A testing kv store that serves as a replicated state machine.
 */
class ReplicationKvStore : public CommandRunnerInterface {
  public:
    ReplicationKvStore();
    ~ReplicationKvStore() override;

    std::string Run(std::string const &command) override;

    void Reset() override;

  private:
    std::unordered_map<std::string, std::string> kvs_;
};

} // namespace e8

#endif // REPLICATION_KEY_VALUE_STORE_H
