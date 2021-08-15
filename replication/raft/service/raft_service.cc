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
#include <grpcpp/grpcpp.h>
#include <mutex>

#include "proto_cc/service_raft.grpc.pb.h"
#include "proto_cc/service_raft.pb.h"
#include "replication/raft/context.h"
#include "replication/raft/election.h"
#include "replication/raft/service/raft_service.h"

namespace e8 {

RaftServiceImpl::RaftServiceImpl(RaftContext *context) : context_(context) {}

RaftServiceImpl::~RaftServiceImpl() {}

grpc::Status RaftServiceImpl::GrantVote(grpc::ServerContext *, GrantVoteRequest const *request,
                                        GrantVoteResponse *response) {
    assert(request->candidate_term() >= 0);

    std::lock_guard<RaftVotingRecord> guard(*context_->voting_record);

    auto [term, _] = context_->role_at_term->TermAndRole();
    if (request->candidate_term() <= term) {
        // We could possibly casted our ballot at the current term.
        response->set_vote_granted(false);
        return grpc::Status::OK;
    }

    context_->role_at_term->UpgradeTerm(context_->me, request->candidate_term(),
                                        RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);

    if (context_->journal->Stale(request->candidate_liveness())) {
        response->set_vote_granted(false);
        return grpc::Status::OK;
    }

    response->set_vote_granted(true);
    return grpc::Status::OK;
}

grpc::Status RaftServiceImpl::MergeLogEntries(grpc::ServerContext *,
                                              MergeLogEntriesRequest const *request,
                                              MergeLogEntriesResponse *response) {
    assert(request->term() >= 0);
    assert(request->overwrite_from() >= 0);

    std::lock_guard<std::mutex> guard(merge_log_entries_lock_);

    auto [term, _] = context_->role_at_term->TermAndRole();
    if (request->term() < term) {
        // Rejects an out-of-date leader to prevent logs get overwritten with obsolete data.
        response->set_successful(false);
        response->set_current_term(term);
        return grpc::Status::OK;
    }

    context_->follower_schedule->ConfirmHeartbeat();
    context_->role_at_term->UpgradeTerm(context_->me, request->term(),
                                        RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);

    bool resolvable = context_->journal->MergeForeignLogs(
        request->overwrite_from(), request->overwrite_with(), request->preceding_term());
    if (!resolvable) {
        // Fails to reconcile with the alien log source.
        response->set_successful(false);
        response->set_current_term(request->term());
        return grpc::Status::OK;
    }

    response->set_successful(true);
    response->set_current_term(request->term());
    return grpc::Status::OK;
}

grpc::Status RaftServiceImpl::PushCommitProgress(grpc::ServerContext *,
                                                 PushCommitProgressRequest const *request,
                                                 PushCommitProgressResponse *) {
    assert(request->term() >= 0);
    assert(request->safe_commit_progress() >= 0);

    std::lock_guard<std::mutex> guard(set_commit_progress_lock_);

    auto [term, _] = context_->role_at_term->TermAndRole();
    if (request->term() < term) {
        // Rejects an out-of-date leader to filter out false heartbeats.
        return grpc::Status::OK;
    }

    context_->follower_schedule->ConfirmHeartbeat();
    context_->role_at_term->UpgradeTerm(context_->me, request->term(),
                                        RoleAtTerm::ENCOUNTERED_HIGHER_TERM_MESSAGE);

    context_->journal->PushCommitProgress(request->safe_commit_progress());

    return grpc::Status::OK;
}

} // namespace e8
