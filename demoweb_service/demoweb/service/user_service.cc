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
#include <string>
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/pagination.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "demoweb_service/demoweb/module/search_user.h"
#include "demoweb_service/demoweb/module/user_identity.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "demoweb_service/demoweb/service/user_service.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/service_user.grpc.pb.h"
#include "proto_cc/service_user.pb.h"
#include "proto_cc/user_relation.pb.h"

namespace e8 {

grpc::Status UserServiceImpl::Register(grpc::ServerContext * /*context*/,
                                       RegistrationRequest const *request,
                                       RegistrationResponse *response) {

    std::optional<UserEntity> user =
        CreateBaselineUser(request->security_key(),
                           /*userId=*/std::nullopt, DemoWebEnvironment()->CurrentHostId(),
                           DemoWebEnvironment()->DemowebDatabase());
    if (!user.has_value()) {
        return grpc::Status(grpc::StatusCode::INTERNAL,
                            "User ID conflicts when it shouldn't happen");
    }

    response->set_user_id(*user->id.Value());
    response->set_error_type(RegistrationResponse::RET_NoError);

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::Authorize(grpc::ServerContext * /*context*/,
                                        AuthorizationRequest const *request,
                                        AuthorizationResponse *response) {
    std::optional<UserEntity> user =
        FetchUser(request->user_id(), DemoWebEnvironment()->DemowebDatabase());
    if (!user.has_value()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND,
                            "User ID=" + std::to_string(request->user_id()) + " doesn't exist.");
    }

    std::optional<SignedIdentity> signed_identity =
        SignIdentity(user.value(), request->security_key(), DemoWebEnvironment()->KeyGen());
    if (!signed_identity.has_value()) {
        return grpc::Status(grpc::StatusCode::UNAUTHENTICATED,
                            "Failed to validate the provided security key.");
    }

    IdentitySignature sig;
    sig.set_signature(signed_identity.value());
    *response->mutable_signed_identity() = sig;

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::GetPublicProfile(grpc::ServerContext *context,
                                               GetPublicProfileRequest const *request,
                                               GetPublicProfileResponse *response) {
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, nullptr);

    std::optional<UserEntity> user =
        FetchUser(request->user_id(), DemoWebEnvironment()->DemowebDatabase());
    if (!user.has_value()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND,
                            "User ID=" + std::to_string(request->user_id()) + " doesn't exist.");
    }

    std::vector<UserPublicProfile> profiles;
    if (identity.has_value()) {
        profiles = BuildPublicProfiles(identity.value().user_id(), {user.value()},
                                       DemoWebEnvironment()->KeyGen(),
                                       DemoWebEnvironment()->DemowebDatabase());
    } else {
        profiles = BuildPublicProfiles(std::optional<UserId>(), {user.value()},
                                       DemoWebEnvironment()->KeyGen(),
                                       DemoWebEnvironment()->DemowebDatabase());
    }
    assert(profiles.size() == 1);
    *response->mutable_profile() = profiles[0];

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::UpdatePublicProfile(grpc::ServerContext *context,
                                                  UpdatePublicProfileRequest const *request,
                                                  UpdatePublicProfileResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }
    UserId user_id = identity.value().user_id();

    std::optional<UserEntity> user = FetchUser(user_id, DemoWebEnvironment()->DemowebDatabase());
    if (!user.has_value()) {
        return grpc::Status(grpc::StatusCode::NOT_FOUND,
                            "User ID=" + std::to_string(user_id) + " doesn't exist.");
    }

    std::optional<std::string> alias =
        request->has_alias() ? std::optional<std::string>(request->alias().value()) : std::nullopt;
    std::optional<std::string> biography =
        request->has_biography() ? std::optional<std::string>(request->biography().value())
                                 : std::nullopt;
    UpdateProfile(alias, biography, &user.value(), DemoWebEnvironment()->DemowebDatabase());

    std::vector<UserPublicProfile> profiles =
        BuildPublicProfiles(user_id, {user.value()}, DemoWebEnvironment()->KeyGen(),
                            DemoWebEnvironment()->DemowebDatabase());
    assert(profiles.size() == 1);
    *response->mutable_profile() = profiles[0];

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::Search(grpc::ServerContext *context, SearchUserRequest const *request,
                                     SearchUserResponse *response) {
    grpc::Status status = ValidatePagination(request->pagination(), kResultPerPageLimit);
    if (!status.ok()) {
        return status;
    }

    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    std::vector<UserEntity> user_entities =
        SearchUser(identity->user_id(), request->query(),
                   /*oneof_user_relations=*/std::unordered_set<UserRelation>(),
                   request->pagination(), DemoWebEnvironment()->DemowebDatabase());

    std::vector<UserPublicProfile> profiles =
        BuildPublicProfiles(identity->user_id(), user_entities, DemoWebEnvironment()->KeyGen(),
                            DemoWebEnvironment()->DemowebDatabase());

    *response->mutable_user_profiles() = {profiles.begin(), profiles.end()};
    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::PrepareNewAvatar(grpc::ServerContext *context,
                                               PrepareNewAvatarRequest const *request,
                                               PrepareNewAvatarResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!status.ok()) {
        return status;
    }

    std::optional<UserEntity> user =
        FetchUser(identity.value().user_id(), DemoWebEnvironment()->DemowebDatabase());
    assert(user.has_value());

    if (!AcceptableProfileAvatarFileFormat(request->file_format())) {
        return grpc::Status(grpc::INVALID_ARGUMENT, "Cannot accept the provided file format.");
    }

    AvatarSetup setup =
        SetUpNewProfileAvatar(user.value(), request->file_format(), DemoWebEnvironment()->KeyGen(),
                              DemoWebEnvironment()->DemowebDatabase());
    response->mutable_avatar_readwrite_access()->set_access_token(setup.avatar_path_access_token);

    return grpc::Status::OK;
}

} // namespace e8
