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

#ifndef RAFT_ELECTION_H
#define RAFT_ELECTION_H

#include <memory>
#include <mutex>
#include <optional>

#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/raft.pb.h"
#include "replication/raft/common_types.h"
#include "replication/raft/peer_set.h"

namespace e8 {

/**
 * @brief The RaftVotingRecord class A mutually exclusive device to record who the node has
 * most recently voted for. The user does not necessarily have to guard this object.
 */
class RaftVotingRecord {
  public:
    RaftVotingRecord();
    ~RaftVotingRecord();

    void lock();
    void unlock();

    /**
     * @brief VoteFor Records that the node has casted ballot for the candidate.
     */
    void VoteFor(RaftMachineAddress candidate_address);

    /**
     * @brief LastVotedFor Returns who the node has last voted for if there has been any.
     */
    std::optional<RaftMachineAddress> LastVotedFor() const;

  private:
    std::mutex lock_;
    std::optional<RaftMachineAddress> last_voted_for_;
};

/**
 * @brief The RaftElectionCommittee class Sets the election rules and provides means to conduct
 * elections.
 */
class RaftElectionCommittee {
  public:
    /**
     * @brief The Result class Tracks and tallies the election result.
     */
    class Result {
      public:
        explicit Result(unsigned quorum_size);
        ~Result();

        /**
         * @brief Won Checks if the candidate has won the election by now.
         */
        bool Won() const;

        /**
         * @brief VoteCount The number of votes the candidate currently receives.
         */
        unsigned VoteCount() const;

        /**
         * @brief AddVote The candidates gets one more vote.
         */
        void AddVote();

      private:
        std::mutex mu_;
        unsigned num_votes_;
        unsigned const quorum_size_;
    };

    /**
     * @brief RaftElectionCommittee Sets up election rules.
     *
     * @param peers Connection to each peer's Raft server.
     * @param quorum_size The number of votes to make up a quorum, should be at least
     * floor(peers->PeerCount()/2) + 1
     * @param election_timeout_millis The *recommended* number of milliseconds to time out a
     * GrantVote() RPC.
     */
    RaftElectionCommittee(RaftPeerSet *peers, unsigned quorum_size,
                          TimeIntervalMillis election_timeout_millis);
    ~RaftElectionCommittee();

    /**
     * @brief StartElection Starts a new election for the specified term. It's OK to run multiple
     * elections at the same term. However, each node other than itself will only be able to cast
     * ballot at most once, and this function doesn't cache past election result. This call does not
     * block. It's up to the caller to determine when it is appropiate to check the election result.
     *
     * @param for_term The term to start election for.
     * @param candidate_machine_address The machine address of the candidate.
     * @param candidate_log_liveness The log's freshness of the candidate node.
     *
     * @return A result object allowing the caller to check the election result whenever it wants.
     */
    std::shared_ptr<Result> StartElection(RaftTerm for_term,
                                          RaftMachineAddress candidate_machine_address,
                                          LogSourceLiveness const &candidate_log_liveness);

  private:
    RaftPeerSet *peers_;
    unsigned const quorum_size_;
    TimeIntervalMillis const election_timeout_millis_;
    ThreadPool thread_pool_;
};

} // namespace e8

#endif // RAFT_ELECTION_H
