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
#include <chrono>
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/raft.pb.h"
#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/journal.h"
#include "replication/raft/journal_replicator.h"
#include "replication/raft/peer_set.h"

namespace e8 {
namespace {

class ReplicationArgs : public TaskStorageInterface {
  public:
    ReplicationArgs(RaftMachineAddress from, RaftTerm publisher_term, RaftJournal const *journal,
                    RaftMachineAddress target_address, RaftService::Stub *target_stub,
                    std::optional<unsigned> current_replication_progress,
                    TimeIntervalMillis rpc_timeout_millis);
    ~ReplicationArgs() override;

    RaftMachineAddress const from;
    RaftTerm const publisher_term;
    RaftJournal const *const journal;
    RaftService::Stub *const target_stub;
    std::optional<unsigned> const current_replication_progress;
    TimeIntervalMillis const rpc_timeout_millis;

    // Return value.
    RaftMachineAddress target_address;
    std::optional<unsigned> latest_replication_progress;
};

ReplicationArgs::ReplicationArgs(RaftMachineAddress from, RaftTerm publisher_term,
                                 RaftJournal const *journal, RaftMachineAddress target_address,
                                 RaftService::Stub *target_stub,
                                 std::optional<unsigned> current_replication_progress,
                                 TimeIntervalMillis rpc_timeout_millis)
    : from(from), publisher_term(publisher_term), journal(journal), target_stub(target_stub),
      current_replication_progress(current_replication_progress),
      rpc_timeout_millis(rpc_timeout_millis), target_address(target_address) {}

ReplicationArgs::~ReplicationArgs() {}

class ReplicationTask : public TaskInterface {
  public:
    ReplicationTask();
    ~ReplicationTask() override;

    void Run(TaskStorageInterface *args) const override;

    bool DropResourceOnCompletion() const override;
};

ReplicationTask::ReplicationTask() {}

ReplicationTask::~ReplicationTask() {}

void ReplicationTask::Run(TaskStorageInterface *data) const {
    auto args = static_cast<ReplicationArgs *>(data);

    LogSourceLiveness liveness = args->journal->Liveness();

    if (args->current_replication_progress.has_value() &&
        *args->current_replication_progress == liveness.log_progress()) {
        // The peer is up-to-date.
        args->latest_replication_progress = liveness.log_progress();
        return;
    }

    int overwrite_from;
    if (!args->current_replication_progress.has_value()) {
        // Don't know what the peer node has. Try overwriting from the end of the journal to
        // minimize request size because, apriori, each peer's journal is almost
        // synchronized.
        overwrite_from = liveness.log_progress();
    } else {
        // Starts from where we left off last time.
        overwrite_from = *args->current_replication_progress;
    }

    for (; overwrite_from >= 0; --overwrite_from) {
        MergeLogEntriesRequest request;
        request.set_term(args->publisher_term);
        request.set_overwrite_from(overwrite_from);

        RaftTerm preceding_term;
        if (!args->journal->Export(overwrite_from, request.mutable_overwrite_with(),
                                   &preceding_term)) {
            // The local journal was shortened by another leader. No point of trying if there is
            // other higher term leaders.
            args->latest_replication_progress = args->current_replication_progress;
            break;
        }
        request.set_preceding_term(preceding_term);

        MergeLogEntriesResponse response;

        grpc::ClientContext context;
        context.set_deadline(std::chrono::system_clock::now() +
                             std::chrono::milliseconds(args->rpc_timeout_millis));

        grpc::Status status = args->target_stub->MergeLogEntries(&context, request, &response);
        if (!status.ok()) {
            // Peer unreachable. No progress.
            args->latest_replication_progress = args->current_replication_progress;
            break;
        }

        if (response.current_term() > args->publisher_term) {
            // Leader is outdated.
            std::cout << "leader outdated: publisher_term=" << args->publisher_term
                      << ", replication_node_term=" << response.current_term() << std::endl;

            args->latest_replication_progress = args->current_replication_progress;
            break;
        }

        if (response.successful()) {
            args->latest_replication_progress = liveness.log_progress();
            break;
        }
    }

    assert(overwrite_from >= 0);
}

bool ReplicationTask::DropResourceOnCompletion() const { return false; }

} // namespace

RaftJournalReplicator::RaftJournalReplicator(RaftJournal const *journal, RaftPeerSet const *peers,
                                             TimeIntervalMillis rpc_timeout_millis)
    : journal_(journal), peers_(peers), thread_pool_(peers->PeerCount()),
      rpc_timeout_millis_(rpc_timeout_millis) {}

RaftJournalReplicator::Progress RaftJournalReplicator::Replicate(RaftMachineAddress from,
                                                                 RaftTerm publisher_term) {
    auto replication_task = std::make_shared<ReplicationTask>();

    for (auto const &[peer_address, peer_stub] : *peers_) {
        if (peer_address == from) {
            // Local journal has all the information already.
            progress_.replication_progresses[from] = journal_->Liveness().log_progress();
            continue;
        }

        std::optional<unsigned> peer_replication_progress;
        auto it = progress_.replication_progresses.find(peer_address);
        if (it != progress_.replication_progresses.end()) {
            peer_replication_progress = it->second;
        }

        auto args = std::make_unique<ReplicationArgs>(from, publisher_term, journal_, peer_address,
                                                      peer_stub.get(), peer_replication_progress,
                                                      rpc_timeout_millis_);
        thread_pool_.Schedule(replication_task, std::move(args));
    }

    // Waits for peer responses and collect new replication progresses.
    for (unsigned i = 0; i < peers_->PeerCount() - 1; ++i) {
        std::unique_ptr<TaskStorageInterface> task_data = thread_pool_.WaitForNextCompleted();
        ReplicationArgs const *reply = static_cast<ReplicationArgs const *>(task_data.get());

        if (reply->latest_replication_progress.has_value()) {
            progress_.replication_progresses[reply->target_address] =
                *reply->latest_replication_progress;
        }
    }

    return progress_;
}

void RaftJournalReplicator::ResetProgress() { progress_.replication_progresses.clear(); }

} // namespace e8
