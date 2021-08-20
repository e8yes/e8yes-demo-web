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
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>

#include "common/time_util/time_util.h"
#include "replication/raft/common_types.h"
#include "replication/raft/integration_test/local_cluster.h"
#include "replication/raft/integration_test/role_monitor.h"

namespace e8 {

namespace role_monitor_internal {

TimeIntervalMillis kInspectionInterval = 20;

class RaftRoleMonitorImpl : public TaskInterface {
  public:
    RaftRoleMonitorImpl(LocalRaftCluster const *cluster);
    ~RaftRoleMonitorImpl();

    void Run(TaskStorageInterface *) const override;

    bool DropResourceOnCompletion() const override;

    void Shutdown();

    std::unordered_map<RaftMachineAddress, RaftRole> HighestTermRoleStatus();

    LocalRaftCluster const *Cluster() const;

  private:
    void Record(RaftTerm term, RaftMachineAddress const &node_address, RaftRole role) const;

    bool done_;
    LocalRaftCluster const *cluster_;
    RaftTerm highest_term_;
    std::unordered_map<RaftTerm, std::unordered_map<RaftMachineAddress, RaftRole>>
        term_role_status_;
    std::mutex mu_;
};

RaftRoleMonitorImpl::RaftRoleMonitorImpl(LocalRaftCluster const *cluster)
    : done_(false), cluster_(cluster), highest_term_(RaftTerm()) {}

RaftRoleMonitorImpl::~RaftRoleMonitorImpl() {}

void RaftRoleMonitorImpl::Record(RaftTerm term, RaftMachineAddress const &node_address,
                                 RaftRole role) const {
    RaftRoleMonitorImpl *mutable_this = const_cast<RaftRoleMonitorImpl *>(this);

    std::lock_guard<std::mutex> guard(mutable_this->mu_);

    mutable_this->highest_term_ = std::max(term, mutable_this->highest_term_);

    auto it = mutable_this->term_role_status_.find(term);
    if (it == mutable_this->term_role_status_.end()) {
        std::unordered_map<RaftMachineAddress, RaftRole> new_term_status(
            {std::make_pair(node_address, role)});
        mutable_this->term_role_status_.insert(std::make_pair(term, new_term_status));
        return;
    }

    auto role_it = it->second.find(node_address);
    if (role_it != it->second.end()) {
        if (role != role_it->second) {
            if (!(role_it->second == RAFT_CANDIDATE && role == RAFT_LEADER)) {
                std::cerr << "RaftRoleMonitorImpl: overwriting existing role at term=" << term
                          << ", from=" << RoleString(role_it->second) << " to= " << RoleString(role)
                          << std::endl;

                assert(false);
            }

            role_it->second = role;
        }
    } else {
        it->second.insert(std::make_pair(node_address, role));
    }

    std::vector<RaftMachineAddress> leaders;
    for (auto const &[node_addr, role] : it->second) {
        if (role == RAFT_LEADER) {
            leaders.push_back(node_addr);
        }
    }

    if (leaders.size() > 1) {
        std::cerr << "RaftRoleMonitorImpl: detecting more than 1 leader, count=" << leaders.size()
                  << "nodes=[" << leaders[0];
        for (unsigned i = 1; i < leaders.size(); ++i) {
            std::cerr << "," << leaders[i];
        }
        std::cerr << "]" << std::endl;

        assert(false);
    }
}

void RaftRoleMonitorImpl::Run(TaskStorageInterface *) const {
    while (!done_) {
        for (auto const &[node_addr, node] : *cluster_) {
            if (node.instance == nullptr) {
                continue;
            }

            auto [term, role] = node.instance->Foreground()->TermAndRole();
            this->Record(term, node_addr, role);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(kInspectionInterval));
    }
}

bool RaftRoleMonitorImpl::DropResourceOnCompletion() const { return false; }

void RaftRoleMonitorImpl::Shutdown() { done_ = true; }

std::unordered_map<RaftMachineAddress, RaftRole> RaftRoleMonitorImpl::HighestTermRoleStatus() {
    std::lock_guard<std::mutex> guard(mu_);

    return term_role_status_[highest_term_];
}

LocalRaftCluster const *RaftRoleMonitorImpl::Cluster() const { return cluster_; }

} // namespace role_monitor_internal

RaftRoleMonitor::RaftRoleMonitor(LocalRaftCluster const *cluster)
    : pimpl_(std::make_shared<role_monitor_internal::RaftRoleMonitorImpl>(cluster)),
      monitor_thread_(/*num_workers=*/1) {
    monitor_thread_.Schedule(pimpl_, /*task_data=*/nullptr);
}

RaftRoleMonitor::~RaftRoleMonitor() {
    pimpl_->Shutdown();
    monitor_thread_.WaitForNextCompleted();
}

std::optional<RaftMachineAddress> RaftRoleMonitor::FindOneRaftLeaeder() {
    for (TimeIntervalMillis time_spent = 0; time_spent < 2 * pimpl_->Cluster()->ElectionTimeout();
         time_spent += 2 * role_monitor_internal::kInspectionInterval) {
        std::unordered_map<RaftMachineAddress, RaftRole> role_status =
            pimpl_->HighestTermRoleStatus();

        for (auto const &[node_addr, role] : role_status) {
            if (role == RAFT_LEADER) {
                return node_addr;
            }
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(2 * role_monitor_internal::kInspectionInterval));
    }

    return std::nullopt;
}

} // namespace e8
