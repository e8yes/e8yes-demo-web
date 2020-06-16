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
#include <grpcpp/grpcpp.h>
#include <optional>
#include <unordered_map>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module_socialnetwork/retrieve_contact.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "demoweb_service/demoweb/proto_cc/service_socialnetwork.grpc.pb.h"
#include "demoweb_service/demoweb/proto_cc/service_socialnetwork.pb.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "demoweb_service/demoweb/service/social_network_service.h"

namespace e8 {

grpc::Status SocialNetworkServiceImpl::GetUserRelations(grpc::ServerContext *context,
                                                        GetUserRelationsRequest const *request,
                                                        GetUserRelationsResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    std::unordered_map<UserId, UserRelations> relations =
        GetUsersRelations(identity.value().user_id(), {request->target_user_id()},
                          CurrentEnvironment()->DemowebDatabase());
    assert(relations.size() == 1);

    UserRelations result = relations.begin()->second;
    *response->mutable_user_relation() = {result.begin(), result.end()};

    return grpc::Status::OK;
}

} // namespace e8
