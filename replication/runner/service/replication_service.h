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

#ifndef REPLICATION_SERVICE_H
#define REPLICATION_SERVICE_H

#include <grpcpp/grpcpp.h>
#include <memory>

#include "proto_cc/replication.pb.h"
#include "proto_cc/service_replication.grpc.pb.h"
#include "proto_cc/service_replication.pb.h"
#include "replication/runner/command_queue.h"
#include "replication/runner/runner.h"

namespace e8 {

/**
 * @brief The ReplicationServiceImpl class Service of a single replication instance provides to
 * replicate command execution sequence amount multiple nodes.
 */
class ReplicationServiceImpl : public ReplicationService::Service {
  public:
    /**
     * @brief ReplicationServiceImpl Specifies the custom runner used to run the committed commands
     * as well as the configuration which defines the behavior of this replication node.
     */
    ReplicationServiceImpl(CommandRunnerInterface *runner, ReplicationConfig const &config);
    ~ReplicationServiceImpl() override;

    grpc::Status RunCommand(grpc::ServerContext *context, RunCommandRequest const *request,
                            RunCommandResponse *response) override;

  private:
    std::unique_ptr<ReplicationInstance> instance_;
};

} // namespace e8

#endif // REPLICATION_SERVICE_H
