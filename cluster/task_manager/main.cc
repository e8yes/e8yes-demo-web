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
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "cluster/task_manager/context.h"
#include "cluster/task_manager/crud.h"
#include "cluster/task_manager/service.h"
#include "common/flags/parse_flags.h"

static char const kToolsBinaryPathFlag[] = "tools_binary_path";
static char const kBuildBinaryPathFlag[] = "build_binary_path";
static char const kTaskStdlogPathFlag[] = "task_stdlog_path";
static char const kTaskHistoryLogPathFlag[] = "task_history_log_path";
static char const kStartupConfigPathFlag[] = "startup_config_path";
static char const kPortFlag[] = "port";

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);

    std::optional<int> port = e8::ReadFlag(kPortFlag, int(), e8::FromString<int>);

    std::optional<std::string> tools_binary_path =
        e8::ReadFlag(kToolsBinaryPathFlag, std::string(), e8::FromString<std::string>);

    std::optional<std::string> build_binary_path =
        e8::ReadFlag(kBuildBinaryPathFlag, std::string(), e8::FromString<std::string>);

    std::optional<std::string> task_stdlog_path =
        e8::ReadFlag(kTaskStdlogPathFlag, std::string(), e8::FromString<std::string>);

    std::optional<std::string> task_history_log_path =
        e8::ReadFlag(kTaskHistoryLogPathFlag, std::string(), e8::FromString<std::string>);

    std::optional<std::string> startup_config_path =
        e8::ReadFlag(kStartupConfigPathFlag, std::string(), e8::FromString<std::string>);

    assert(port.has_value());
    assert(tools_binary_path.has_value());
    assert(build_binary_path.has_value());
    assert(task_stdlog_path.has_value());
    assert(task_history_log_path.has_value());
    assert(startup_config_path.has_value());

    std::unique_ptr<e8::TaskManagerContext> context =
        e8::CreateTaskManagerContext(*tools_binary_path, *build_binary_path, *task_stdlog_path,
                                     *task_history_log_path, *startup_config_path);

    e8::LaunchStartupTasks(context.get());

    auto task_manager_service = std::make_unique<e8::TaskManagerServiceImpl>(std::move(context));

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    std::string server_address("0.0.0.0:" + std::to_string(*port));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(task_manager_service.get());

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();

    return 0;
}
