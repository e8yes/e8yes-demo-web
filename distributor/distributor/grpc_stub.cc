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

#include <string>

#include "distributor/distributor/grpc_stub.h"
#include "proto_cc/node.pb.h"

namespace e8 {
namespace grpc_stub_internal {

std::string IpStr(std::string const &ip_bytes) {
    if (ip_bytes.size() == 4) {
        return std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[0]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[1]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[2]))) +
               "." +
               std::to_string(static_cast<unsigned int>(static_cast<unsigned char>(ip_bytes[3])));
    } else {
        // TODO: Add IPv6 support.
        assert(false);
    }
}

std::string NodeToTargetStr(NodeState const &target, int port) {
    return IpStr(target.ip_address()) + ":" + std::to_string(port);
}

} // namespace grpc_stub_internal
} // namespace e8
