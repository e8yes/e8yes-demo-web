#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

static int const kDefaultPort = 50051;

std::optional<int> port_option(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string option(argv[i]);
        if (option.rfind("--port=", 0) == 0) {
            std::string port = option.substr(std::string("--port=").length());
            return std::stoi(port);
        }
    }
    return std::nullopt;
}

int main(int argc, char *argv[]) {
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    std::optional<int> port = port_option(argc, argv);
    if (!port.has_value()) {
        port = kDefaultPort;
    }
    std::string server_address("0.0.0.0:" + std::to_string(port.value()));
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
    return 0;
}
