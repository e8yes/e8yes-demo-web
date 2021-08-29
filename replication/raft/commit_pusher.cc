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

#include <algorithm>
#include <cassert>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <vector>

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/commit_pusher.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/journal_replicator.h"
#include "replication/raft/peer_set.h"

namespace e8 {
namespace {

class PushCommitArgs : public TaskStorageInterface {
  public:
    PushCommitArgs(RaftMachineAddress const &pusher, RaftTerm pusher_term,
                   unsigned safe_commit_progress, RaftJournal *journal,
                   RaftService::Stub *target_stub, bool use_rpc,
                   TimeIntervalMillis rpc_timeout_millis);
    ~PushCommitArgs() override;

    RaftMachineAddress const pusher;
    RaftTerm const pusher_term;
    unsigned const safe_commit_progress;
    RaftJournal *journal;
    RaftService::Stub *const target_stub;
    bool const use_rpc;
    TimeIntervalMillis const rpc_timeout_millis;
};

PushCommitArgs::PushCommitArgs(RaftMachineAddress const &pusher, RaftTerm pusher_term,
                               unsigned safe_commit_progress, RaftJournal *journal,
                               RaftService::Stub *target_stub, bool use_rpc,
                               TimeIntervalMillis rpc_timeout_millis)
    : pusher(pusher), pusher_term(pusher_term), safe_commit_progress(safe_commit_progress),
      journal(journal), target_stub(target_stub), use_rpc(use_rpc),
      rpc_timeout_millis(rpc_timeout_millis) {}

PushCommitArgs::~PushCommitArgs() {}

class PushCommitTask : public TaskInterface {
  public:
    PushCommitTask();
    ~PushCommitTask() override;

    void Run(TaskStorageInterface *args) const override;

    bool DropResourceOnCompletion() const override;
};

PushCommitTask::PushCommitTask() {}

PushCommitTask::~PushCommitTask() {}

void PushCommitTask::Run(TaskStorageInterface *data) const {
    auto args = static_cast<PushCommitArgs const *>(data);

    if (args->use_rpc) {
        PushCommitProgressRequest request;
        request.set_term(args->pusher_term);
        request.set_safe_commit_progress(args->safe_commit_progress);

        PushCommitProgressResponse response;

        grpc::ClientContext context;
        context.set_deadline(std::chrono::system_clock::now() +
                             std::chrono::milliseconds(args->rpc_timeout_millis));

        args->target_stub->PushCommitProgress(&context, request, &response);
    } else {
        args->journal->PushCommitProgress(args->safe_commit_progress);
    }
}

bool PushCommitTask::DropResourceOnCompletion() const { return true; }

unsigned QuorumReplicationRrogress(RaftJournalReplicator::Progress const &progress,
                                   unsigned quorum_size) {
    std::vector<unsigned> progresses(progress.replication_progresses.size());
    unsigned i = 0;
    for (auto const &[_, p] : progress.replication_progresses) {
        progresses[i++] = p;
    }

    std::sort(progresses.begin(), progresses.end(), std::greater<unsigned>());

    if (progresses.size() < quorum_size) {
        // We haven't known enough nodes' replication progress. We'll simply assume they made zero
        // progress.
        return 0;
    }

    return progresses[quorum_size - 1];
}

} // namespace

RaftCommitPusher::RaftCommitPusher(RaftJournal *journal, RaftPeerSet const *peers,
                                   TimeIntervalMillis rpc_timeout_millis)
    : journal_(journal), peers_(peers), thread_pool_(peers->PeerCount()),
      rpc_timeout_millis_(rpc_timeout_millis) {}

RaftCommitPusher::~RaftCommitPusher() {}

void RaftCommitPusher::Push(RaftMachineAddress const &pusher, RaftTerm pusher_term,
                            RaftJournalReplicator::Progress const &progress) {
    // The global commit progress is determined based on the minimum replication progress of the
    // most up-to-date quorum and bounded by the portion of logs ended with current term addition.
    unsigned quorum_progress = QuorumReplicationRrogress(progress, peers_->QuorumSize());
    unsigned global_commit_progress = journal_->EndWithTerm(pusher_term, quorum_progress);

    auto task = std::make_shared<PushCommitTask>();

    for (auto const &[peer_address, peer_stub] : *peers_) {
        // Determines what commit progress is guaranteed to be safe for each individual peer.
        unsigned safe_commit_progress;

        auto replication_progress_it = progress.replication_progresses.find(peer_address);
        if (replication_progress_it == progress.replication_progresses.end()) {
            safe_commit_progress = 0;
        } else {
            safe_commit_progress =
                std::min(global_commit_progress, replication_progress_it->second);
        }

        // The local node is called directly to save bandwidth.
        bool use_rpc = peer_address != pusher;

        auto args =
            std::make_unique<PushCommitArgs>(pusher, pusher_term, safe_commit_progress, journal_,
                                             peer_stub.get(), use_rpc, rpc_timeout_millis_);
        thread_pool_.Schedule(task, std::move(args));
    }
}

} // namespace e8