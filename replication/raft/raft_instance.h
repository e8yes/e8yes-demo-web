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

#ifndef RAFT_INSTANCE_H
#define RAFT_INSTANCE_H

#include <memory>

#include "common/thread/thread_pool.h"
#include "replication/raft/background.h"
#include "replication/raft/context.h"
#include "replication/raft/foreground.h"
#include "replication/raft/journal.h"

namespace e8 {

/**
 * @brief The RaftInstance class Encapsulates the handling of Raft foreground, background task as
 * well as the context they share. The client of this library should always interact using only this
 * class.
 */
class RaftInstance {
  public:
    /**
     * @brief RaftInstance Runs a new Raft instance with the specified commit listener and
     * configuration. By the end of the construction, a Raft background will be running and will
     * only terminate when this instance is destructed.
     */
    RaftInstance(RaftCommitListener *commit_listener, RaftConfig const &config);
    ~RaftInstance();

    /**
     * @brief Foreground Retrieves the Raft foreground to manipulate command boardcast.
     */
    RaftForeground *Foreground() const;

  private:
    std::unique_ptr<RaftContext> context_;
    std::unique_ptr<RaftForeground> foreground_;
    std::shared_ptr<RaftBackground> background_;

    ThreadPool background_thread_;
};

} // namespace e8

#endif // RAFT_INSTANCE_H
