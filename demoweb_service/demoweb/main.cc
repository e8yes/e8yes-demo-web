/**
 * e8yes demo web server.
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
#include <cstdlib>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "common/flags/parse_flags.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/environment/prod_environment_context.h"
#include "demoweb_service/demoweb/service/file_service.h"
#include "demoweb_service/demoweb/service/message_channel_service.h"
#include "demoweb_service/demoweb/service/social_network_service.h"
#include "demoweb_service/demoweb/service/user_service.h"
#include "message_queue/subscriber/environment/environment_context_interface.h"
#include "message_queue/subscriber/environment/prod_environment_context.h"
#include "message_queue/subscriber/service/message_subscriber_service.h"

namespace {

static char const kPortFlag[] = "port";
static char const kGrpcWebProxyFlag[] = "grpc_web_proxy";
static char const kDemowebDbHostNameFlag[] = "demoweb_db_host_name";
static char const kNodeStateDbPathFlag[] = "node_state_db_path";
static char const kMessageQueueServicePortFlag[] = "message_queue_service_port";

static int const kDefaultPort = 50051;

static e8::UserServiceImpl gUserService;
static e8::FileServiceImpl gFileService;
static e8::SocialNetworkServiceImpl gSocialNetworkService;
static e8::MessageChannelServiceImpl gMessageChannelService;
static e8::MessageSubscriberServiceImpl gMessageSubscriberService;

std::unique_ptr<e8::DemoWebProductionEnvironmentContext> BuildDemoWebEnvironmentContext() {
    std::string demoweb_db_host_name =
        e8::ReadFlag(kDemowebDbHostNameFlag, std::string(), e8::FromString<std::string>);
    assert(!demoweb_db_host_name.empty());

    auto context = std::make_unique<e8::DemoWebProductionEnvironmentContext>(demoweb_db_host_name);

    return context;
}

std::unique_ptr<e8::SubscriberProductionEnvironmentContext>
BuildMessageSubscriberEnvironmentContext() {
    std::string demoweb_db_host_name =
        e8::ReadFlag(kDemowebDbHostNameFlag, std::string(), e8::FromString<std::string>);
    assert(!demoweb_db_host_name.empty());

    std::string node_state_db_path =
        e8::ReadFlag(kNodeStateDbPathFlag, std::string(), e8::FromString<std::string>);
    assert(!node_state_db_path.empty());

    e8::MessageQueueServicePort message_queue_service_port = e8::ReadFlag(
        kMessageQueueServicePortFlag, e8::MessageQueueServicePort(), e8::FromString<uint32_t>);
    assert(message_queue_service_port != 0);

    auto context = std::make_unique<e8::SubscriberProductionEnvironmentContext>(
        node_state_db_path, demoweb_db_host_name, message_queue_service_port);

    return context;
}

} // namespace

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);

    auto demo_web_environment = BuildDemoWebEnvironmentContext();
    e8::RegisterEnvironment(demo_web_environment.get());

    auto message_subscriber_environment = BuildMessageSubscriberEnvironmentContext();
    e8::RegisterEnvironment(message_subscriber_environment.get());

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    int port = e8::ReadFlag<int>(kPortFlag, kDefaultPort, e8::FromString<int>);
    std::string server_address("0.0.0.0:" + std::to_string(port));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&gUserService);
    builder.RegisterService(&gFileService);
    builder.RegisterService(&gSocialNetworkService);
    builder.RegisterService(&gMessageChannelService);
    builder.RegisterService(&gMessageSubscriberService);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    // Run GRPC web proxy.
    std::string grpc_web_proxy =
        e8::ReadFlag(kGrpcWebProxyFlag, std::string(), e8::FromString<std::string>);
    if (!grpc_web_proxy.empty()) {
        std::string cmd = grpc_web_proxy + " --backend_addr=localhost:50051 --run_tls_server=false "
                                           "--server_http_debug_port=8000 --allow_all_origins &";
        int _ = std::system(cmd.c_str());
        (void)_;
    }

    server->Wait();

    return 0;
}
