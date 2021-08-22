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

#include <grpcpp/grpcpp.h>
#include <memory>

#include "proto_cc/replication.pb.h"
#include "proto_cc/service_replication.grpc.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/runner.h"
#include "replication/runner/service/replication_service.h"

namespace e8 {

ReplicationServiceImpl::ReplicationServiceImpl(CommandRunnerInterface *runner,
                                               ReplicationConfig const &config)
    : instance_(std::make_unique<ReplicationInstance>(runner, config)) {}

ReplicationServiceImpl::~ReplicationServiceImpl() {}

grpc::Status ReplicationServiceImpl::RunCommand(grpc::ServerContext * /*context*/,
                                                RunCommandRequest const *request,
                                                RunCommandResponse *response) {
    ReplicationInstance::RunCommandResult result =
        instance_->RunCommand(request->command(), request->run_event_id());

    response->set_error(result.error);
    if (result.retry_node.has_value()) {
        response->mutable_retry_address()->set_value(*result.retry_node);
    }
    response->set_return_value(result.return_value);

    return grpc::Status::OK;
}

} // namespace e8
