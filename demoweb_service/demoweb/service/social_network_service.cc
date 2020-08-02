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
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/pagination.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module/contact_invitation.h"
#include "demoweb_service/demoweb/module/retrieve_contact.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/service_socialnetwork.grpc.pb.h"
#include "proto_cc/service_socialnetwork.pb.h"
#include "proto_cc/user_profile.pb.h"
#include "proto_cc/user_relation.pb.h"
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
    response->set_source_user_id(identity.value().user_id());
    response->set_target_user_id(request->target_user_id());
    *response->mutable_user_relations() = {result.begin(), result.end()};

    return grpc::Status::OK;
}

grpc::Status SocialNetworkServiceImpl::SendInvitation(grpc::ServerContext *context,
                                                      SendInvitationRequest const *request,
                                                      SendInvitationResponse * /*response*/) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    ::e8::SendInvitation(identity.value().user_id(), request->invitee_user_id(),
                         /*send_message_anyway=*/true, CurrentEnvironment()->DemowebDatabase());

    return grpc::Status::OK;
}

grpc::Status SocialNetworkServiceImpl::GetRelatedUserList(grpc::ServerContext *context,
                                                          GetRelatedUserListRequest const *request,
                                                          GetRelatedUserListResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    status = ValidatePagination(request->pagination(), kResultPerPageLimit);
    if (!status.ok()) {
        return status;
    }

    std::vector<UserRelation> relation_filter(request->relation_filter_size());
    for (int i = 0; i < request->relation_filter_size(); i++) {
        relation_filter[i] = request->relation_filter(i);
    }
    std::vector<UserEntity> inviters =
        GetRelatedUsers(identity.value().user_id(), relation_filter, request->pagination(),
                        CurrentEnvironment()->DemowebDatabase());
    std::vector<UserPublicProfile> inviter_profiles =
        BuildPublicProfiles(identity.value().user_id(), inviters, CurrentEnvironment()->KeyGen(),
                            CurrentEnvironment()->DemowebDatabase());

    *response->mutable_user_profiles() = {inviter_profiles.begin(), inviter_profiles.end()};

    return grpc::Status::OK;
}

grpc::Status SocialNetworkServiceImpl::ProcessInvitation(grpc::ServerContext *context,
                                                         ProcessInvitationRequest const *request,
                                                         ProcessInvitationResponse * /*response*/) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    if (!e8::ProcessInvitation(identity->user_id(), request->inviter_user_id(), request->accept(),
                               CurrentEnvironment()->DemowebDatabase())) {
        return grpc::Status(grpc::StatusCode::FAILED_PRECONDITION, "Invitation does not exist.");
    }

    return grpc::Status::OK;
}

} // namespace e8
