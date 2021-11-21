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
#include <memory>
#include <string>

#include "cluster/conductor/background.h"
#include "cluster/conductor/conductor.h"
#include "cluster/conductor/instance.h"
#include "cluster/conductor/revision_work_pool.h"
#include "common/thread/thread_pool.h"
#include "proto_cc/cluster_conductor.pb.h"
#include "proto_cc/cluster_conductor_command.pb.h"
#include "proto_cc/cluster_revision_command.pb.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/runner.h"

namespace e8 {
namespace {

class ConductorStores : public CommandRunnerInterface {
  public:
    ConductorStores(std::string const &revision_work_pool_snapshot_file);
    ~ConductorStores() override;

    std::string Run(std::string const &command) override;

  private:
    std::unique_ptr<ClusterRevisionWorkPool> revision_work_pool_;
};

ConductorStores::ConductorStores(std::string const &revision_work_pool_snapshot_file)
    : revision_work_pool_(
          std::make_unique<ClusterRevisionWorkPool>(revision_work_pool_snapshot_file)) {}

ConductorStores::~ConductorStores() {}

std::string ConductorStores::Run(std::string const &command) {
    ClusterConductorCommand conductor_command;
    conductor_command.ParseFromString(command);

    ClusterConductorCommandResult result;

    switch (conductor_command.command_case()) {
    case ClusterConductorCommand::CommandCase::kRevision: {
        ClusterRevisionResult revision_result =
            revision_work_pool_->Run(conductor_command.revision());
        *result.mutable_revision_result() = revision_result;
        break;
    }
    default: {
        assert(false);
    }
    }

    return result.SerializeAsString();
}

} // namespace

struct ConductorInstance::ConductorInstanceImpl {
    ConductorInstanceImpl(ClusterConductorConfig const &config);
    ~ConductorInstanceImpl();

    std::unique_ptr<ConductorStores> stores_;
    std::unique_ptr<ReplicationInstance> conductor_replicator_;
    std::unique_ptr<ClusterRevisionConductor> revision_conductor_;
    std::shared_ptr<ClusterConductorBackground> background_;
    std::unique_ptr<ThreadPool> background_thread_;
};

ConductorInstance::ConductorInstanceImpl::ConductorInstanceImpl(
    ClusterConductorConfig const &config)
    : stores_(std::make_unique<ConductorStores>(config.revision_work_pool_snapshot_file())),
      conductor_replicator_(
          std::make_unique<ReplicationInstance>(stores_.get(), config.replication_config())),
      revision_conductor_(std::make_unique<ClusterRevisionConductor>(conductor_replicator_.get())),
      background_(std::make_shared<ClusterConductorBackground>(revision_conductor_.get())),
      background_thread_(std::make_unique<ThreadPool>(/*hardware_concurrency=*/1)) {

    background_thread_->Schedule(background_);
}

ConductorInstance::ConductorInstanceImpl::~ConductorInstanceImpl() {
    background_->Shutdown();
    background_thread_->WaitForNextCompleted();
}

ConductorInstance::ConductorInstance(ClusterConductorConfig const &config)
    : pimpl_(std::make_unique<ConductorInstanceImpl>(config)) {}

ConductorInstance::~ConductorInstance() {}

} // namespace e8
