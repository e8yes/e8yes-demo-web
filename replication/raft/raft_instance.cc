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

#include <memory>

#include "common/thread/thread_pool.h"
#include "replication/raft/background.h"
#include "replication/raft/context.h"
#include "replication/raft/foreground.h"
#include "replication/raft/journal.h"
#include "replication/raft/raft_instance.h"

namespace e8 {

RaftInstance::RaftInstance(RaftCommitListener *commit_listener, RaftConfig const &config)
    : context_(CreateRaftContext(commit_listener, config)),
      foreground_(std::make_unique<RaftForeground>(context_.get())),
      background_(std::make_shared<RaftBackground>(context_.get())),
      background_thread_(/*num_workers=*/1) {
    background_thread_.Schedule(background_);
}

RaftInstance::~RaftInstance() {
    background_->Shutdown();
    background_thread_.WaitForNextCompleted();
}

RaftForeground *RaftInstance::Foreground() const { return foreground_.get(); }

} // namespace e8
