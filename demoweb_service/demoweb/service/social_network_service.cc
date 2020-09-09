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
#include <grpcpp/grpcpp.h>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/pagination.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module/contact_invitation.h"
#include "demoweb_service/demoweb/module/contact_storage.h"
#include "demoweb_service/demoweb/module/search_user.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "demoweb_service/demoweb/service/social_network_service.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/service_socialnetwork.grpc.pb.h"
#include "proto_cc/service_socialnetwork.pb.h"
#include "proto_cc/user_profile.pb.h"
#include "proto_cc/user_relation.pb.h"

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
                          DemoWebEnvironment()->DemowebDatabase());
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
                         /*send_message_anyway=*/true, DemoWebEnvironment()->CurrentHostId(),
                         DemoWebEnvironment()->ClientPushMessagePublishers(),
                         DemoWebEnvironment()->KeyGen(), DemoWebEnvironment()->DemowebDatabase());

    return grpc::Status::OK;
}

grpc::Status
SocialNetworkServiceImpl::SearchRelatedUserList(grpc::ServerContext *context,
                                                SearchRelatedUserListRequest const *request,
                                                SearchRelatedUserListResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    status = ValidatePagination(request->pagination(), kResultPerPageLimit);
    if (!status.ok()) {
        return status;
    }

    std::unordered_set<UserRelation> relation_filter;
    for (auto const relation : request->relation_filter()) {
        relation_filter.insert(static_cast<UserRelation>(relation));
    }
    std::optional<std::string> search_terms =
        request->has_search_terms() ? std::optional<std::string>(request->search_terms().value())
                                    : std::nullopt;

    std::vector<UserEntity> related_users =
        SearchUser(identity.value().user_id(), search_terms, relation_filter, request->pagination(),
                   DemoWebEnvironment()->DemowebDatabase());
    std::vector<UserPublicProfile> related_profiles = BuildPublicProfiles(
        identity.value().user_id(), related_users, DemoWebEnvironment()->KeyGen(),
        DemoWebEnvironment()->DemowebDatabase());

    *response->mutable_user_profiles() = {related_profiles.begin(), related_profiles.end()};

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
                               DemoWebEnvironment()->CurrentHostId(),
                               DemoWebEnvironment()->ClientPushMessagePublishers(),
                               DemoWebEnvironment()->KeyGen(),
                               DemoWebEnvironment()->DemowebDatabase())) {
        return grpc::Status(grpc::StatusCode::FAILED_PRECONDITION, "Invitation does not exist.");
    }

    return grpc::Status::OK;
}

grpc::Status SocialNetworkServiceImpl::DeleteContact(grpc::ServerContext *context,
                                                     DeleteContactRequest const *request,
                                                     DeleteContactResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    bool deleted = e8::DeleteContact(identity->user_id(), request->deleted_contact_user_id(),
                                     DemoWebEnvironment()->DemowebDatabase());
    response->set_deleted(deleted);

    return grpc::Status::OK;
}

} // namespace e8
