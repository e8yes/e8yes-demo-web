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
#include <memory>

#include "common/thread/thread_pool.h"
#include "replication/raft/background.h"
#include "replication/raft/common_types.h"
#include "replication/raft/context.h"

namespace e8 {
namespace {

std::unique_ptr<grpc::Server> StartRaftServer(RaftMachineAddress const &machine_address) {
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    grpc::ServerBuilder builder;
    builder.AddListeningPort(machine_address, grpc::InsecureServerCredentials());
    // TODO: register Raft service here when it is implemented.

    return builder.BuildAndStart();
}

void ShutdownRaftServer(std::unique_ptr<grpc::Server> const &server) {
    assert(server == nullptr);

    server->Shutdown();
    server->Wait();
}

} // namespace

RaftBackground::RaftBackground(std::shared_ptr<RaftContext> const &context) : context_(context) {}

RaftBackground::~RaftBackground() {}

void RaftBackground::Shutdown() { done_ = true; }

void RaftBackground::Run(TaskStorageInterface *) const {
    auto raft_server = StartRaftServer(context_->me);

    while (!done_) {
        // TODO: Plays the current raft role.
    }

    ShutdownRaftServer(raft_server);
}

bool RaftBackground::DropResourceOnCompletion() const { return true; }

} // namespace e8
