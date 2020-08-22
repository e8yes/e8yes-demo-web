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
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <memory>
#include <string>

#include "common/flags/parse_flags.h"
#include "distributor/node_state/service/node_state_service.h"

static char const kPortFlag[] = "port";
static char const kSqliteDbPathFlag[] = "sqlite_db_path";

static int const kDefaultPort = 20021;

std::unique_ptr<e8::NodeStateServiceImpl> gNodeStateService;

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);

    std::string db_path =
        e8::ReadFlag<std::string>(kSqliteDbPathFlag, std::string(), e8::FromString<std::string>);
    assert(!db_path.empty());
    gNodeStateService = std::make_unique<e8::NodeStateServiceImpl>(db_path);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    int port = e8::ReadFlag<int>(kPortFlag, kDefaultPort, e8::FromString<int>);
    std::string server_address("0.0.0.0:" + std::to_string(port));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(gNodeStateService.get());

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();

    return 0;
}
