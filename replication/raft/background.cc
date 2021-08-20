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
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <iostream>
#include <memory>

#include "common/thread/thread_pool.h"
#include "replication/raft/background.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"
#include "replication/raft/election.h"
#include "replication/raft/service/raft_service.h"

namespace e8 {
namespace {

struct ServerAndService {
    std::unique_ptr<grpc::Server> server;
    std::unique_ptr<RaftServiceImpl> service;
};

std::string SwapTo0000(RaftMachineAddress const &machine_address) {
    std::size_t pos = machine_address.find_last_of(":");
    assert(pos > 0);
    assert(pos < machine_address.size() - 1);
    std::string port_suffix = machine_address.substr(pos, machine_address.size());

    return std::string("0.0.0.0") + port_suffix;
}

ServerAndService StartRaftServer(RaftContext *context) {
    auto service = std::make_unique<RaftServiceImpl>(context);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    grpc::ServerBuilder builder;
    std::string listening_address = SwapTo0000(context->me);
    builder.AddListeningPort(listening_address, grpc::InsecureServerCredentials());
    builder.RegisterService(service.get());

    ServerAndService result;
    result.server = builder.BuildAndStart();
    result.service = std::move(service);

    return result;
}

void ShutdownRaftServer(grpc::Server *server) {
    assert(server != nullptr);

    server->Shutdown();
    server->Wait();
}

void PrepareForLeadership(RaftContext *context) { context->journal_replicator->ResetProgress(); }

void DoFollower(RaftContext *context) {
    if (!context->follower_schedule->WaitForHeartbeat()) {
        auto [current_term, _] = context->role_at_term->TermAndRole();
        context->role_at_term->UpgradeTerm(context->me, current_term + 1,
                                           RoleAtTerm::LACK_OF_HEARTBEAT);
    }
}

void DoCandidate(RaftContext *context, RaftTerm term_to_establish) {
    std::shared_ptr<RaftElectionCommittee::Result> election_result =
        context->election_committee->StartElection(term_to_establish, context->me,
                                                   context->journal->Liveness());
    context->candidate_schedule->WaitForElectionResult();

    if (election_result->Won()) {
        std::cout << "At node=" << context->me
                  << ": won an election with #votes=" << election_result->VoteCount()
                  << " for term=" << term_to_establish << std::endl;

        PrepareForLeadership(context);
        context->role_at_term->UpgradeTerm(context->me, term_to_establish,
                                           RoleAtTerm::WON_AN_ELECTION);
    } else {
        std::cout << "At node=" << context->me
                  << ": lost an election with #votes=" << election_result->VoteCount()
                  << " for term=" << term_to_establish << std::endl;

        context->role_at_term->UpgradeTerm(context->me, term_to_establish + 1,
                                           RoleAtTerm::LOST_AN_ELECTION);
    }
}

void DoLeader(RaftContext *context, RaftTerm leader_term) {
    context->leader_schedule->BeginWork();

    RaftJournalReplicator::Progress progress =
        context->journal_replicator->Replicate(context->me, leader_term);
    context->commit_pusher->Push(context->me, leader_term, progress);

    context->leader_schedule->Sleep();
}

} // namespace

RaftBackground::RaftBackground(RaftContext *context) : context_(context), done_(false) {}

RaftBackground::~RaftBackground() {}

void RaftBackground::Shutdown() { done_ = true; }

void RaftBackground::Run(TaskStorageInterface *) const {
    std::cout << "At node=" << context_->me << ": background task started." << std::endl;

    ServerAndService server_and_service = StartRaftServer(context_);

    while (!done_) {
        auto [term, role] = context_->role_at_term->TermAndRole();

        switch (role) {
        case RAFT_FOLLOWER: {
            DoFollower(context_);
            break;
        }
        case RAFT_CANDIDATE: {
            DoCandidate(context_, term);
            break;
        }
        case RAFT_LEADER: {
            DoLeader(context_, term);
            break;
        }
        default: {
            assert(false);
        }
        }
    }

    ShutdownRaftServer(server_and_service.server.get());

    std::cout << "At node=" << context_->me << ": background task shutdown." << std::endl;
}

bool RaftBackground::DropResourceOnCompletion() const { return false; }

} // namespace e8
