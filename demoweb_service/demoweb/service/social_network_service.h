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

#ifndef SOCIAL_NETWORK_SERVICE_H
#define SOCIAL_NETWORK_SERVICE_H

#include <grpcpp/grpcpp.h>

#include "demoweb_service/demoweb/proto_cc/service_socialnetwork.grpc.pb.h"

namespace e8 {

/**
 * @brief The SocialNetworkServiceImpl class Service for user relation management.
 */
class SocialNetworkServiceImpl : public SocialNetworkService::Service {
  public:
    SocialNetworkServiceImpl() = default;
    ~SocialNetworkServiceImpl() override = default;

    grpc::Status GetUserRelations(grpc::ServerContext *context,
                                  GetUserRelationsRequest const *request,
                                  GetUserRelationsResponse *response) override;

    grpc::Status SendInvitation(grpc::ServerContext *context, SendInvitationRequest const *request,
                                SendInvitationResponse *response) override;

    grpc::Status GetRelatedUserList(grpc::ServerContext *context,
                                    GetRelatedUserListRequest const *request,
                                    GetRelatedUserListResponse *response) override;

    grpc::Status ProcessInvitation(grpc::ServerContext *context,
                                   ProcessInvitationRequest const *request,
                                   ProcessInvitationResponse *response) override;
};

} // namespace e8

#endif // SOCIAL_NETWORK_SERVICE_H
