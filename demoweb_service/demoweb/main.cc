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

#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/environment/prod_environment_context.h"
#include "demoweb_service/demoweb/service/file_service.h"
#include "demoweb_service/demoweb/service/social_network_service.h"
#include "demoweb_service/demoweb/service/user_service.h"

namespace {

static char const kPortFlag[] = "port";
static char const kDemowebDbHostNameFlag[] = "demoweb_db_host_name";
static char const kDemowebDbPortFlag[] = "demoweb_db_port";
static char const kDemowebDbUserName[] = "demoweb_db_user_name";
static char const kDemowebDbPassword[] = "demoweb_db_password";

static int const kDefaultPort = 50051;

static e8::UserServiceImpl gUserService;
static e8::FileServiceImpl gFileService;
static e8::SocialNetworkServiceImpl gSocialNetworkService;

std::optional<std::string> ScanFlag(int argc, char *argv[], std::string const &flag) {
    std::string expected_flag_prefix = "--" + flag + "=";

    for (int i = 1; i < argc; i++) {
        std::string cur_flag(argv[i]);
        if (cur_flag.rfind(expected_flag_prefix, 0) == 0) {
            std::string flag_value = cur_flag.substr(std::string(expected_flag_prefix).length());
            return flag_value;
        }
    }

    return std::nullopt;
}

int PortValue(int argc, char *argv[]) {
    std::optional<std::string> port_val = ScanFlag(argc, argv, kPortFlag);
    if (port_val.has_value()) {
        return std::stoi(port_val.value());
    } else {
        return kDefaultPort;
    }
}

std::unique_ptr<e8::ProductionEnvironmentContext> BuildEnvironmentContext(int argc, char *argv[]) {
    std::optional<std::string> demoweb_db_host_name = ScanFlag(argc, argv, kDemowebDbHostNameFlag);
    std::optional<std::string> demoweb_db_port = ScanFlag(argc, argv, kDemowebDbPortFlag);
    std::optional<std::string> demoweb_db_user_name = ScanFlag(argc, argv, kDemowebDbUserName);
    std::optional<std::string> demoweb_db_password = ScanFlag(argc, argv, kDemowebDbPassword);
    assert(demoweb_db_host_name.has_value());
    assert(demoweb_db_port.has_value());
    assert(demoweb_db_user_name.has_value());
    assert(demoweb_db_password.has_value());

    auto context = std::make_unique<e8::ProductionEnvironmentContext>(
        demoweb_db_host_name.value(), std::stoi(demoweb_db_port.value()),
        demoweb_db_user_name.value(), demoweb_db_password.value());

    return context;
}

} // namespace

int main(int argc, char *argv[]) {
    auto prod_environment = BuildEnvironmentContext(argc, argv);
    e8::RegisterEnvironment(prod_environment.get());

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    int port = PortValue(argc, argv);
    std::string server_address("0.0.0.0:" + std::to_string(port));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&gUserService);
    builder.RegisterService(&gFileService);
    builder.RegisterService(&gSocialNetworkService);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Run GRPC web proxy.
    std::optional<std::string> grpc_web_proxy = ScanFlag(argc, argv, "grpc_web_proxy");
    if (grpc_web_proxy.has_value()) {
        std::string cmd = grpc_web_proxy.value() +
                          " --backend_addr=localhost:50051 --run_tls_server=false "
                          "--server_http_debug_port=8000 --allow_all_origins &";
        std::system(cmd.c_str());
    }

    server->Wait();

    return 0;
}
