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
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
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
        request->has_description() ? std::optional<std::string>(request->description().value())
                                   : std::nullopt;
    std::vector<UserId> to_be_member_ids{request->member_ids().begin(),
                                         request->member_ids().end()};

    MessageChannelEntity channel = ::e8::CreateMessageChannel(
        identity->user_id(), channel_title, channel_desc, to_be_member_ids, request->encrypted(),
        request->close_group_channel(), DemoWebEnvironment()->CurrentHostId(),
        DemoWebEnvironment()->DemowebDatabase());

    response->set_channel_id(*channel.id.Value());

    return grpc::Status::OK;
}

grpc::Status
MessageChannelServiceImpl::UpdateMessageChannel(grpc::ServerContext *context,
                                                UpdateMessageChannelRequest const *request,
                                                UpdateMessageChannelResponse * /*response*/) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<std::string> channel_title =
        request->has_title() ? std::optional<std::string>(request->title().value()) : std::nullopt;
    std::optional<std::string> channel_desc =
        request->has_description() ? std::optional<std::string>(request->description().value())
                                   : std::nullopt;
    if (!UpdateMessageChannelMetadata(identity->user_id(), request->channel_id(), channel_title,
                                      channel_desc, DemoWebEnvironment()->MessageChannelPbac(),
                                      DemoWebEnvironment()->DemowebDatabase())
             .has_value()) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                            "Failed to update the message channel's metadata.");
    }

    std::vector<MessageChannelMembership> membership_proposal{request->memberships().begin(),
                                                              request->memberships().end()};
    if (!UpdateMessageChannelMembership(
            identity->user_id(), request->channel_id(), membership_proposal,
            DemoWebEnvironment()->MessageChannelPbac(), DemoWebEnvironment()->DemowebDatabase())) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                            "Failed to apply the entire/part of the membership proposal.");
    }

    return grpc::Status::OK;
}

grpc::Status
MessageChannelServiceImpl::LeaveMessageChannel(grpc::ServerContext *context,
                                               LeaveMessageChannelRequest const *request,
                                               LeaveMessageChannelResponse * /*response*/) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    MessageChannelMembership removal;
    removal.set_channel_id(request->channel_id());
    removal.set_user_id(identity->user_id());

    if (!UpdateMessageChannelMembership(identity->user_id(), request->channel_id(), {removal},
                                        DemoWebEnvironment()->MessageChannelPbac(),
                                        DemoWebEnvironment()->DemowebDatabase())) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                            "Failed to apply the membership removal proposal.");
    }

    return grpc::Status::OK;
}

grpc::Status
MessageChannelServiceImpl::SearchMessageChannels(grpc::ServerContext *context,
                                                 SearchMessageChannelsRequest const *request,
                                                 SearchMessageChannelsResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::unordered_set<UserId> contains_member_ids{request->with_member_ids().begin(),
                                                   request->with_member_ids().end()};
    std::unordered_set<MessageChannelId> any_channel_ids{request->channel_ids().begin(),
                                                         request->channel_ids().end()};
    std::optional<std::string> query_text =
        request->has_search_text() ? std::optional<std::string>(request->search_text().value())
                                   : std::nullopt;
    std::optional<Pagination> pagination =
        request->has_pagination() ? std::optional<Pagination>(request->pagination()) : std::nullopt;

    std::vector<SearchedMessageChannel> channels = ::e8::SearchMessageChannels(
        identity->user_id(), contains_member_ids, any_channel_ids, query_text,
        request->active_member_fetch_limit(), pagination, DemoWebEnvironment()->DemowebDatabase());

    std::vector<MessageChannelOverview> results =
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

} // namespace e8
