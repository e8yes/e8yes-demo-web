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
#include <memory>
#include <mutex>
#include <optional>

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/raft.pb.h"
#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/election.h"
#include "replication/raft/peer_set.h"

namespace e8 {
namespace {

class CollectVoteArgs : public TaskStorageInterface {
  public:
    CollectVoteArgs(RaftTerm for_term, RaftMachineAddress const &candidate_machine_address,
                    LogSourceLiveness const &candidate_log_liveness, RaftService::Stub *target_stub,
                    TimeIntervalMillis election_timeout_millis,
                    std::shared_ptr<RaftElectionCommittee::Result> const &election_result);
    virtual ~CollectVoteArgs() override;

    RaftTerm for_term;
    RaftMachineAddress candidate_machine_address;
    LogSourceLiveness candidate_log_liveness;
    RaftService::Stub *target_stub;
    TimeIntervalMillis election_timeout_millis;
    std::shared_ptr<RaftElectionCommittee::Result> election_result;
};

CollectVoteArgs::CollectVoteArgs(
    RaftTerm for_term, RaftMachineAddress const &candidate_machine_address,
    LogSourceLiveness const &candidate_log_liveness, RaftService::Stub *target_stub,
    TimeIntervalMillis election_timeout_millis,
    std::shared_ptr<RaftElectionCommittee::Result> const &election_result)
    : for_term(for_term), candidate_machine_address(candidate_machine_address),
      candidate_log_liveness(candidate_log_liveness), target_stub(target_stub),
      election_timeout_millis(election_timeout_millis), election_result(election_result) {}

CollectVoteArgs::~CollectVoteArgs() {}

class CollectVoteTask : public TaskInterface {
  public:
    CollectVoteTask();
    ~CollectVoteTask();

    void Run(TaskStorageInterface *args) const override;

    bool DropResourceOnCompletion() const override;

  private:
    std::shared_ptr<RaftElectionCommittee::Result> election_result_;
};

CollectVoteTask::CollectVoteTask() {}

CollectVoteTask::~CollectVoteTask() {}

void CollectVoteTask::Run(TaskStorageInterface *data) const {
    auto args = static_cast<CollectVoteArgs const *>(data);

    GrantVoteRequest request;
    request.set_candidate_machine_address(args->candidate_machine_address);
    request.set_candidate_term(args->for_term);
    *request.mutable_candidate_liveness() = args->candidate_log_liveness;

    GrantVoteResponse response;

    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() +
                         std::chrono::milliseconds(args->election_timeout_millis));

    grpc::Status status = args->target_stub->GrantVote(&context, request, &response);
    if (status.ok() || !response.vote_granted()) {
        return;
    }

    args->election_result->AddVote();
}

bool CollectVoteTask::DropResourceOnCompletion() const { return false; }

} // namespace

RaftVotingRecord::RaftVotingRecord() {}

RaftVotingRecord::~RaftVotingRecord() {}

void RaftVotingRecord::lock() { return lock_.lock(); }

void RaftVotingRecord::unlock() { return lock_.unlock(); }

void RaftVotingRecord::VoteFor(RaftMachineAddress candidate_address) {
    last_voted_for_ = candidate_address;
}

std::optional<RaftMachineAddress> RaftVotingRecord::LastVotedFor() const { return last_voted_for_; }

RaftElectionCommittee::RaftElectionCommittee(RaftPeerSet *peers, unsigned quorum_size,
                                             TimeIntervalMillis election_timeout_millis)
    : peers_(peers), quorum_size_(quorum_size), election_timeout_millis_(election_timeout_millis),
      thread_pool_(peers->PeerCount()) {
    assert(quorum_size >= peers->PeerCount() / 2 + 1);
    assert(quorum_size <= peers->PeerCount());
}

RaftElectionCommittee::~RaftElectionCommittee() {}

std::shared_ptr<RaftElectionCommittee::Result>
RaftElectionCommittee::StartElection(RaftTerm for_term,
                                     RaftMachineAddress candidate_machine_address,
                                     LogSourceLiveness const &candidate_log_liveness) {
    auto result = std::make_shared<Result>(quorum_size_);

    auto collect_vote_task = std::make_shared<CollectVoteTask>();

    for (auto const &[peer_address, peer_stub] : *peers_) {
        if (peer_address == candidate_machine_address) {
            // Votes for ourselves.
            result->AddVote();
            continue;
        }

        auto args = std::make_unique<CollectVoteArgs>(for_term, candidate_machine_address,
                                                      candidate_log_liveness, peer_stub.get(),
                                                      election_timeout_millis_, result);
        thread_pool_.Schedule(collect_vote_task, std::move(args));
    }

    return result;
}

RaftElectionCommittee::Result::Result(unsigned quorum_size) : quorum_size_(quorum_size) {}

RaftElectionCommittee::Result::~Result() {}

bool RaftElectionCommittee::Result::Won() const { return num_votes_ >= quorum_size_; }

unsigned RaftElectionCommittee::Result::VoteCount() const { return num_votes_; }

void RaftElectionCommittee::Result::AddVote() {
    std::lock_guard<std::mutex> guard(mu_);
    ++num_votes_;
}

} // namespace e8
