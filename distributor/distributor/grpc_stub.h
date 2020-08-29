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

#ifndef GRPC_STUB_H
#define GRPC_STUB_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "proto_cc/node.pb.h"

namespace e8 {
namespace grpc_stub_internal {

std::string NodeToTargetStr(NodeState const &target, int port);

} // namespace grpc_stub_internal

/**
 * Create a grpc stub to the service given the host location and port number.
 */
#define CREATE_GRPC_STUB(service_type__, target__, port__)                                         \
    (service_type__::NewStub(                                                                      \
        grpc::CreateChannel(grpc_stub_internal::NodeToTargetStr(target__, port__),                 \
                            grpc::InsecureChannelCredentials())))

} // namespace e8

#endif // GRPC_STUB_H
