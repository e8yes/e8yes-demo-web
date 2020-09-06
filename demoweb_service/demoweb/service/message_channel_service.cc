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

#include <algorithm>
#include <grpcpp/grpcpp.h>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/service/message_channel_service.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_profile.pb.h"

namespace e8 {

grpc::Status
MessageChannelServiceImpl::CreateMessageChannel(grpc::ServerContext *context,
                                                CreateMessageChannelRequest const *request,
                                                CreateMessageChannelResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<std::string> channel_title =
        request->has_title() ? std::optional<std::string>(request->title().value()) : std::nullopt;
    std::optional<std::string> channel_desc =
        request->has_title() ? std::optional<std::string>(request->description().value())
                             : std::nullopt;
    std::vector<UserId> to_be_member_ids{request->member_ids().begin(),
                                         request->member_ids().end()};

    std::optional<MessageChannelEntity> channel = ::e8::CreateMessageChannel(
        identity->user_id(), channel_title, channel_desc, to_be_member_ids, request->encrypted(),
        request->close_group_channel(), DemoWebEnvironment()->CurrentHostId(),
        DemoWebEnvironment()->DemowebDatabase());
    if (channel.has_value()) {
        return grpc::Status(grpc::StatusCode::UNKNOWN, "Channel creation failed");
    }

    response->set_channel_id(*channel->id.Value());

    return grpc::Status::OK;
}

grpc::Status MessageChannelServiceImpl::GetJoinedInMessageChannels(
    grpc::ServerContext *context, GetJoinedInMessageChannelsRequest const *request,
    GetJoinedInMessageChannelsResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::vector<UserId> contains_member_ids{request->with_member_ids().begin(),
                                            request->with_member_ids().end()};
    if (std::find(contains_member_ids.begin(), contains_member_ids.end(), identity->user_id()) ==
        contains_member_ids.end()) {
        contains_member_ids.push_back(identity->user_id());
    }

    std::optional<Pagination> pagination =
        request->has_pagination() ? std::optional<Pagination>(request->pagination()) : std::nullopt;

    std::vector<SearchedMessageChannel> channels =
        ::e8::SearchMessageChannels(contains_member_ids, request->active_member_fetch_limit(),
                                    pagination, DemoWebEnvironment()->DemowebDatabase());

    std::vector<MessageChannelOveriew> results =
        ToMessageChannelOverviews(identity->user_id(), channels, DemoWebEnvironment()->KeyGen(),
                                  DemoWebEnvironment()->DemowebDatabase());
    *response->mutable_channels() = {results.begin(), results.end()};

    return grpc::Status::OK;
}

grpc::Status
MessageChannelServiceImpl::GetMessageChannelMembers(grpc::ServerContext *context,
                                                    GetMessageChannelMembersRequest const *request,
                                                    GetMessageChannelMembersResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<Pagination> pagination =
        request->has_pagination() ? std::optional<Pagination>(request->pagination()) : std::nullopt;

    std::vector<MessageChannelMember> members = ::e8::GetMessageChannelMembers(
        identity->user_id(), pagination, DemoWebEnvironment()->DemowebDatabase());

    // Build member profiles.
    std::vector<UserEntity> users(members.size());
    for (unsigned i = 0; i < members.size(); i++) {
        users[i] = members[i].member;
    }
    std::vector<UserPublicProfile> profiles =
        BuildPublicProfiles(identity->user_id(), users, DemoWebEnvironment()->KeyGen(),
                            DemoWebEnvironment()->DemowebDatabase());
    *response->mutable_user_profiles() = {profiles.begin(), profiles.end()};

    // Build channel relation.
    std::vector<MessageChannelRelation> relations(members.size());
    for (unsigned i = 0; i < members.size(); i++) {
        MessageChannelRelation relation;
        relation.set_member_type(members[i].member_type);
        relation.set_join_at(members[i].join_at);

        relations[i] = relation;
    }
    *response->mutable_channel_relations() = {relations.begin(), relations.end()};

    return grpc::Status::OK;
}

grpc::Status
MessageChannelServiceImpl::AddUserToMessageChannel(grpc::ServerContext *context,
                                                   AddUserToMessageChannelRequest const *request,
                                                   AddUserToMessageChannelResponse * /*response*/) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    if (!e8::AddUserToMessageChannel(identity->user_id(), request->channel_id(), request->user_id(),
                                     request->member_type(),
                                     DemoWebEnvironment()->DemowebDatabase())) {
        return grpc::Status(
            grpc::StatusCode::PERMISSION_DENIED,
            "Can't add the person as a channel member with the requested privilege.");
    }

    return grpc::Status::OK;
}

} // namespace e8
