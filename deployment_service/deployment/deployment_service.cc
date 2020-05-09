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

#include <grpcpp/grpcpp.h>

#include "deployment_service/deployment/deployment_service.h"
#include "deployment_service/deployment/proto_cc/deployment_service.grpc.pb.h"
#include "deployment_service/deployment/proto_cc/deployment_service.pb.h"

namespace e8 {

grpc::Status DeploymentServiceImpl::Deploy(grpc::ServerContext *context,
                                           DeployRequest const *request,
                                           grpc::ServerWriter<DeployResponse> *writer) {}

} // namespace e8
