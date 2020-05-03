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

#ifndef DEPLOYMENT_SERVICE_H
#define DEPLOYMENT_SERVICE_H

#include <grpcpp/grpcpp.h>

#include "deployment_service.grpc.pb.h"

namespace e8 {

/**
 * @brief The DeploymentServiceImpl class
 */
class DeploymentServiceImpl : public DeploymentService::Service {
  public:
    grpc::Status Deploy(grpc::ServerContext *context, DeployRequest const *request,
                        grpc::ServerWriter<DeployResponse> *writer) override;
};

} // namespace e8

#endif // DEPLOYMENT_SERVICE_H
