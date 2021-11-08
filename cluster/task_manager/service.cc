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
#include <grpcpp/grpcpp.h>
#include <memory>

#include "cluster/task_manager/context.h"
#include "cluster/task_manager/crud.h"
#include "cluster/task_manager/service.h"
#include "proto_cc/service_task_manager.grpc.pb.h"
#include "proto_cc/service_task_manager.pb.h"

namespace e8 {

TaskManagerServiceImpl::TaskManagerServiceImpl(std::unique_ptr<TaskManagerContext> &&context)
    : context_(std::move(context)) {}

TaskManagerServiceImpl::~TaskManagerServiceImpl() {}

grpc::Status TaskManagerServiceImpl::LaunchTask(grpc::ServerContext * /*context*/,
                                                LaunchTaskRequest const *request,
                                                LaunchTaskResponse *response) {
    LaunchTaskResult result = e8::LaunchTask(request->launch_config(), context_.get());
    response->set_error(result.error);
    response->set_os_return_code(result.os_return_code);
    *response->mutable_task() = result.task_info;

    return grpc::Status::OK;
}

grpc::Status TaskManagerServiceImpl::TerminateTask(grpc::ServerContext * /*context*/,
                                                   TerminateTaskRequest const *request,
                                                   TerminateTaskResponse *response) {
    TerminateTaskResult result = e8::TerminateTask(request->task_id(), context_.get());
    response->set_error(result.error);
    response->set_os_return_code(result.os_return_code);

    return grpc::Status::OK;
}

} // namespace e8
