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
#include <chrono>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <vector>

#include "cluster/conductor/boardcast.h"
#include "cluster/conductor/condutor.h"
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

TimeIntervalMillis const kStatusUpdateIntervalMin = 5000;
TimeIntervalMillis const kStatusUpdateIntervalMax = 10000;
TimeIntervalMillis const kStatusUpdateTimeout = 2 * kStatusUpdateIntervalMax;

TimeIntervalMillis RandomizedStatusUpdateInterval(RandomSource *random_source) {
    std::vector<int> rv = DrawUniformInts(kStatusUpdateIntervalMin, kStatusUpdateIntervalMax,
                                          /*num_instances=*/1, random_source);
    return TimeIntervalMillis(rv[0]);
}

struct BoardcastArgs : public TaskStorageInterface {
    BoardcastArgs(Machine const *target_machine, ClusterMapRevision const &revision,
                  ClusterRevisionConductor const &this_conductor);
    ~BoardcastArgs();

    RandomSource random_source;

    // Task args.
    Machine const *target_machine;
    ClusterMapRevision const &revision;
    ClusterRevisionConductor const &this_conductor;

    // Task return values.
    bool dropped;
    bool successful;
};

BoardcastArgs::BoardcastArgs(Machine const *target_machine, ClusterMapRevision const &revision,
                             ClusterRevisionConductor const &this_conductor)
    : target_machine(target_machine), revision(revision), this_conductor(this_conductor),
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

    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() +
                         std::chrono::milliseconds(kStatusUpdateTimeout));

    ApplyClusterMapRevisionRequest request;
    TimeIntervalMillis status_update_interval =
        RandomizedStatusUpdateInterval(&args->random_source);
    request.set_status_update_interval_millis(status_update_interval);
    *request.mutable_revision() = args->revision;

    std::unique_ptr<grpc::ClientReader<ApplyClusterMapRevisionResponse>> stream =
        target_stub->ApplyClusterMapRevision(&context, request);

    ApplyClusterMapRevisionResponse response;
    while (stream->Read(&response)) {
        // TODO: Currently the progress information is discarded. It may be instead stored for
        // progress check by the client of the BoardcastRevision() function.
    }

    args->successful = response.successful();
}

bool BoardcastTask::DropResourceOnCompletion() const { return false; }

} // namespace

bool BoardcastRevision(ClusterMapRevision const &revision,
                       std::vector<Machine> const &target_machines, float rate,
                       ClusterRevisionConductor const &this_conductor,
                       std::vector<Machine> *unsuccessful_machines) {
    unsigned num_parallel_boardcasts =
        std::max(1u, static_cast<unsigned>(target_machines.size() * rate));

    ThreadPool boardcast_pool(num_parallel_boardcasts);

    auto task = std::make_shared<BoardcastTask>();
    for (unsigned i = 0; i < target_machines.size(); ++i) {
        auto args = std::make_unique<BoardcastArgs>(&target_machines[i], revision, this_conductor);
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

bool BoardcastRevisionWithRetry(ClusterMapRevision const &revision,
                                std::vector<Machine> target_machines, float rate,
                                ClusterRevisionConductor const &this_conductor,
                                unsigned num_retries, std::vector<Machine> *unsuccessful_machines) {
    for (unsigned i = 0; i < num_retries + 1; ++i) {
        bool completed = BoardcastRevision(revision, target_machines, rate, this_conductor,
                                           unsuccessful_machines);
        if (!completed) {
            return false;
        }

        target_machines = *unsuccessful_machines;
        unsuccessful_machines->clear();
    }

    return true;
}

} // namespace e8
