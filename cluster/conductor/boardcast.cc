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
#include <optional>
#include <vector>

#include "cluster/conductor/boardcast.h"
#include "cluster/conductor/condutor.h"
#include "cluster/conductor/revision_store.h"
#include "cluster/placement/common_types.h"
#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "common/thread/thread_pool.h"
#include "common/time_util/time_util.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"
#include "proto_cc/service_resource_worker.grpc.pb.h"
#include "proto_cc/service_resource_worker.pb.h"

namespace e8 {
namespace {

TimeIntervalMillis const kStatusUpdateIntervalMin = 100;
TimeIntervalMillis const kStatusUpdateIntervalMax = 200;

TimeIntervalMillis RandomizedStatusUpdateInterval(RandomSource *random_source) {
    std::vector<int> rv = DrawUniformInts(kStatusUpdateIntervalMin, kStatusUpdateIntervalMax,
                                          /*num_instances=*/1, random_source);
    return TimeIntervalMillis(rv[0]);
}

ApplyClusterMapRevisionResponse ApplyClusterMapRevision(ClusterMapRevision const &revision,
                                                        RandomSource *random_source,
                                                        ResourceWorkerService::Stub *target_stub) {
    grpc::ClientContext context;

    ApplyClusterMapRevisionRequest request;
    TimeIntervalMillis status_update_interval = RandomizedStatusUpdateInterval(random_source);
    request.set_status_update_interval_millis(status_update_interval);
    *request.mutable_revision() = revision;

    std::unique_ptr<grpc::ClientReader<ApplyClusterMapRevisionResponse>> stream =
        target_stub->ApplyClusterMapRevision(&context, request);

    ApplyClusterMapRevisionResponse response;
    while (stream->Read(&response)) {
        // TODO: Currently the progress information is discarded. It may be instead stored for
        // progress check by the client of the BoardcastRevision() function.

        // TODO: Timeout the RPC if we haven't received any status update for several update
        // intervals.
    }

    return response;
}

struct BoardcastArgs : public TaskStorageInterface {
    BoardcastArgs(Machine const *target_machine, std::vector<ClusterMapRevision> const &revisions,
                  ClusterMapVersionEpoch wip_starting_version,
                  ClusterRevisionConductorInterface const &this_conductor);
    ~BoardcastArgs();

    RandomSource random_source;

    // Task args.
    Machine const *target_machine;
    std::vector<ClusterMapRevision> const &revisions;
    ClusterMapVersionEpoch const wip_from_version_epoch;
    ClusterRevisionConductorInterface const &this_conductor;

    // Task return values.
    bool dropped;
    bool successful;
};

BoardcastArgs::BoardcastArgs(Machine const *target_machine,
                             std::vector<ClusterMapRevision> const &revisions,
                             ClusterMapVersionEpoch wip_from_version_epoch,
                             ClusterRevisionConductorInterface const &this_conductor)
    : target_machine(target_machine), revisions(revisions),
      wip_from_version_epoch(wip_from_version_epoch), this_conductor(this_conductor),
      dropped(false), successful(false) {}

BoardcastArgs::~BoardcastArgs() {}

class BoardcastTask : public TaskInterface {
  public:
    BoardcastTask();
    ~BoardcastTask();

    void Run(TaskStorageInterface *storage) const;

    bool DropResourceOnCompletion() const;
};

BoardcastTask::BoardcastTask() {}

BoardcastTask::~BoardcastTask() {}

void BoardcastTask::Run(TaskStorageInterface *data) const {
    BoardcastArgs *args = static_cast<BoardcastArgs *>(data);

    if (!args->this_conductor.ShouldBoardcast()) {
        args->dropped = true;
        return;
    }

    std::unique_ptr<ResourceWorkerService::Stub> target_stub = ResourceWorkerService::NewStub(
        grpc::CreateChannel(args->target_machine->address(), grpc::InsecureChannelCredentials()));

    // Our initial guess is the target machine's resource service got updated to the last revision
    // applied to this conductor. If this guess isn't correct, the target's resource service will
    // guide us to the revision from which it starts missing.
    ApplyClusterMapRevisionResponse response;
    ClusterMapVersionEpoch require_from_version_epoch = args->wip_from_version_epoch;
    do {
        std::optional<ClusterMapRevision> revision = MergeClusterMapRevisions(
            args->revisions, require_from_version_epoch, args->revisions.size());
        assert(revision.has_value());
        response = ApplyClusterMapRevision(*revision, &args->random_source, target_stub.get());

        require_from_version_epoch = response.require_from_version_epoch();
    } while (response.require_from_version_epoch() >= 0 &&
             response.require_from_version_epoch() < args->revisions.rbegin()->to_version_epoch());

    // The operation to the machine is considered successful if either the ApplyClusterMapRevision()
    // RPC was successful or the machine is as/even more up-to-date then this conductor.
    args->successful = response.successful() || response.require_from_version_epoch() >=
                                                    args->revisions.rbegin()->to_version_epoch();
}

bool BoardcastTask::DropResourceOnCompletion() const { return false; }

bool BoardcastRevision(std::vector<ClusterMapRevision> const &revision_history,
                       ClusterMapVersionEpoch wip_from_version_epoch,
                       std::vector<Machine> const &target_machines, float rate,
                       ClusterRevisionConductorInterface const &this_conductor,
                       std::vector<Machine> *unsuccessful_machines) {
    assert(wip_from_version_epoch < static_cast<int>(revision_history.size()));

    if (revision_history.empty()) {
        // Nothing to boardcast.
        return true;
    }

    unsigned num_parallel_boardcasts =
        std::max(1u, static_cast<unsigned>(target_machines.size() * rate));

    ThreadPool boardcast_pool(num_parallel_boardcasts);

    auto task = std::make_shared<BoardcastTask>();
    for (unsigned i = 0; i < target_machines.size(); ++i) {
        auto args = std::make_unique<BoardcastArgs>(&target_machines[i], revision_history,
                                                    wip_from_version_epoch, this_conductor);
        boardcast_pool.Schedule(task, std::move(args));
    }

    bool dropped = false;
    unsuccessful_machines->clear();

    for (unsigned i = 0; i < target_machines.size(); ++i) {
        std::unique_ptr<TaskStorageInterface> completed = boardcast_pool.WaitForNextCompleted();
        auto result = static_cast<BoardcastArgs const *>(completed.get());

        if (result->dropped) {
            dropped = true;
        }

        if (!result->successful) {
            unsuccessful_machines->push_back(*result->target_machine);
        }
    }

    return !dropped;
}

std::vector<Machine> TargetMachines(ClusterRevisionStore::RevisionSpecs const &revision_specs) {
    std::vector<Machine> target_machines = revision_specs.cluster_map.Machines();

    if (revision_specs.wip_from_version_epoch < 0) {
        return target_machines;
    }

    for (unsigned i = revision_specs.wip_from_version_epoch; i < revision_specs.revisions.size();
         ++i) {
        for (auto const &action : revision_specs.revisions[i].actions()) {
            if (action.operation() == ClusterMapRevision::ADD_TREE_NODE &&
                action.tree_node().has_machine()) {
                target_machines.push_back(action.tree_node().machine());
            }
        }
    }

    return target_machines;
}

} // namespace

bool BoardcastRevision(ClusterRevisionStore::RevisionSpecs const &revision_specs, float rate,
                       ClusterRevisionConductorInterface const &this_conductor,
                       std::vector<Machine> *unsuccessful_machines) {
    return BoardcastRevision(revision_specs.revisions, revision_specs.wip_from_version_epoch,
                             TargetMachines(revision_specs), rate, this_conductor,
                             unsuccessful_machines);
}

bool BoardcastRevisionWithRetry(ClusterRevisionStore::RevisionSpecs const &revision_specs,
                                float rate, ClusterRevisionConductorInterface const &this_conductor,
                                unsigned num_retries, std::vector<Machine> *unsuccessful_machines) {
    std::vector<Machine> target_machines = TargetMachines(revision_specs);
    for (unsigned i = 0; i < num_retries + 1; ++i) {
        bool completed =
            BoardcastRevision(revision_specs.revisions, revision_specs.wip_from_version_epoch,
                              target_machines, rate, this_conductor, unsuccessful_machines);
        if (!completed) {
            return false;
        }

        target_machines = *unsuccessful_machines;
        unsuccessful_machines->clear();
    }

    return true;
}

} // namespace e8
