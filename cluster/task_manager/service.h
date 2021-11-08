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

#ifndef CLUSTER_TASK_MANAGER_SERVICE_H
#define CLUSTER_TASK_MANAGER_SERVICE_H

#include <grpcpp/grpcpp.h>
#include <memory>

#include "cluster/task_manager/context.h"
#include "proto_cc/service_task_manager.grpc.pb.h"
#include "proto_cc/service_task_manager.pb.h"

namespace e8 {

/**
 * @brief The TaskManagerServiceImpl class A service to manage locally running tasks (programs).
 */
class TaskManagerServiceImpl : public TaskManagerService::Service {
  public:
    TaskManagerServiceImpl(std::unique_ptr<TaskManagerContext> &&context);
    ~TaskManagerServiceImpl() override;

    grpc::Status LaunchTask(grpc::ServerContext *context, LaunchTaskRequest const *request,
                            LaunchTaskResponse *response) override;

    grpc::Status TerminateTask(grpc::ServerContext *context, TerminateTaskRequest const *request,
                               TerminateTaskResponse *response) override;

  private:
    std::unique_ptr<TaskManagerContext> context_;
};

} // namespace e8

#endif // CLUSTER_TASK_MANAGER_SERVICE_H
