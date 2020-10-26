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
#include <cassert>
#include <cstdint>
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/message_channel.pb.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_profile.pb.h"

namespace e8 {
namespace message_channel_internal {

MessageChannelMembershipDelta ComputeMessageChannelMembershipDelta(
    MessageChannelId const channel_id,
    std::vector<MessageChannelMembership> const &proposed_memberships,
    ConnectionReservoirInterface *conns) {
    SqlQueryBuilder message_channel_member_query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    message_channel_member_query.QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu")
        .QueryPiece(" WHERE mchu.channel_id=")
        .Holder(&channel_id_ph);

    message_channel_member_query.SetValueToPlaceholder(channel_id_ph,
                                                       std::make_shared<SqlLong>(channel_id));

    std::vector<std::tuple<MessageChannelHasUserEntity>> members =
        Query<MessageChannelHasUserEntity>(message_channel_member_query, {"mchu"}, conns);
    std::unordered_map<UserId, MessageChannelHasUserEntity> current_members;
    for (auto const &member : members) {
        MessageChannelHasUserEntity const &entity = std::get<0>(member);
        current_members[*entity.user_id.Value()] = entity;
    }

    std::unordered_map<UserId, MessageChannelMembership> proposed_members;
    for (auto const &membership : proposed_memberships) {
        proposed_members[membership.user_id()] = membership;
    }

    MessageChannelMembershipDelta delta;
    for (auto const &[member_id, current_member] : current_members) {
        auto it = proposed_members.find(member_id);
        if (it == proposed_members.end()) {
            // To be removed.
            MessageChannelMembership to_be_removed;
            to_be_removed.set_user_id(*current_member.user_id.Value());
            to_be_removed.set_channel_id(*current_member.channel_id.Value());
            to_be_removed.set_member_type(
                static_cast<MessageChannelMemberType>(*current_member.ownership.Value()));
            delta.to_be_removed.push_back(to_be_removed);
        } else {
            if (it->second.member_type() != *current_member.ownership.Value()) {
                // To be modified.
                delta.to_be_modified.push_back(it->second);
            }
        }
    }
    for (auto const &[proposed_member_id, proposed_member] : proposed_members) {
        auto it = current_members.find(proposed_member_id);
        if (it == current_members.end()) {
            // To be added.
            delta.to_be_added.push_back(proposed_member);
        }
    }

    return delta;
}

} // namespace message_channel_internal

namespace {

std::vector<std::vector<MessageChannelHasUserEntity>>
FetchMostActiveMembers(std::vector<MessageChannelId> const &message_channel_ids,
                       unsigned active_member_fetch_limit, ConnectionReservoirInterface *conns) {
    if (active_member_fetch_limit == 0) {
        return std::vector<std::vector<MessageChannelHasUserEntity>>(message_channel_ids.size());
    }

    SqlQueryBuilder member_query;
    SqlQueryBuilder::Placeholder<SqlLongArr> channel_ids_ph;
    member_query.QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu WHERE mchu.channel_id=ANY(")
        .Holder(&channel_ids_ph)
        .QueryPiece(")");

    member_query.SetValueToPlaceholder(channel_ids_ph,
                                       std::make_shared<SqlLongArr>(message_channel_ids));

    std::vector<std::tuple<MessageChannelHasUserEntity>> members =
        Query<MessageChannelHasUserEntity>(member_query, {"mchu"}, conns);

    // Group by channel ID.
    std::unordered_map<MessageChannelId, std::vector<MessageChannelHasUserEntity>> group_by_channel;
    for (auto const &entry : members) {
        MessageChannelHasUserEntity const &member = std::get<0>(entry);
        group_by_channel[*member.channel_id.Value()].push_back(member);
    }

    std::vector<std::vector<MessageChannelHasUserEntity>> result(message_channel_ids.size());
    for (unsigned i = 0; i < message_channel_ids.size(); ++i) {
        MessageChannelId channel_id = message_channel_ids[i];

        // Use last interaction timestamp as a criteria for activeness.
        std::vector<MessageChannelHasUserEntity> *members = &group_by_channel[channel_id];
        std::sort(members->begin(), members->end(),
                  [](MessageChannelHasUserEntity const &a, MessageChannelHasUserEntity const &b) {
                      return *a.last_interaction_at.Value() > *b.last_interaction_at.Value();
                  });

        // Trim to the limit and write out the IDs.
        std::vector<MessageChannelHasUserEntity>::iterator end_it;
        if (active_member_fetch_limit < members->size()) {
            end_it = members->begin() + active_member_fetch_limit;
            result[i].resize(active_member_fetch_limit);
        } else {
            end_it = members->end();
            result[i].resize(members->size());
        }

        std::copy(members->begin(), end_it, result[i].begin());
    }

    return result;
}

MessageChannel GetMessageChannel(SearchedMessageChannel const &searched_message_channel) {
    MessageChannel proto_message;
    proto_message.set_channel_id(*searched_message_channel.message_channel.id.Value());
    proto_message.set_title(*searched_message_channel.message_channel.channel_name.Value());
    proto_message.set_description(*searched_message_channel.message_channel.description.Value());
    proto_message.set_created_at(*searched_message_channel.message_channel.created_at.Value());
    return proto_message;
}

MessageChannelRelation
GetMessageChannelRelation(SearchedMessageChannel const &searched_message_channel) {
    MessageChannelRelation relation;
    relation.set_join_at(searched_message_channel.join_at);
    relation.set_member_type(searched_message_channel.member_type);
    return relation;
}

} // namespace

MessageChannelEntity CreateMessageChannel(UserId creator_id,
                                          std::optional<std::string> const &channel_name,
                                          std::optional<std::string> const &description,
                                          std::vector<UserId> const &to_be_member_ids,
                                          bool const encrypted, bool const close_group_channel,
                                          HostId const host_id,
                                          ConnectionReservoirInterface *conns) {
    std::time_t current_timestamp;
    std::time(&current_timestamp);

    MessageChannelEntity message_channel = CreateMessageChannel(
        channel_name, description, encrypted, close_group_channel, host_id, conns);

    UpdateMessageChannelMembership(*message_channel.id.Value(), creator_id, MCMT_ADMIN, conns);
    for (UserId const user_id : to_be_member_ids) {
        UpdateMessageChannelMembership(*message_channel.id.Value(), user_id, MCMT_ADMIN, conns);
    }

    return message_channel;
}

std::optional<MessageChannelEntity> UpdateMessageChannelMetadata(
    UserId const viewer_id, MessageChannelId const channel_id,
    std::optional<std::string> const &channel_name, std::optional<std::string> const &description,
    MessageChannelPbacInterface *pbac, ConnectionReservoirInterface *conns) {
    if (!pbac->AllowUpdateChannelMetadata(viewer_id, channel_id)) {
        return std::nullopt;
    }
    return UpdateMessageChannel(channel_id, channel_name, description, conns);
}

bool UpdateMessageChannelMembership(
    UserId const viewer_id, MessageChannelId const channel_id,
    std::vector<MessageChannelMembership> const &proposed_memberships,
    MessageChannelPbacInterface *pbac, ConnectionReservoirInterface *conns) {
    bool all_successful = true;

    message_channel_internal::MessageChannelMembershipDelta delta =
        message_channel_internal::ComputeMessageChannelMembershipDelta(channel_id,
                                                                       proposed_memberships, conns);

    // Evalulate access control.
    for (auto const &membership : delta.to_be_modified) {
        assert(membership.channel_id() == channel_id);
        all_successful &= pbac->AllowUpdateChannelMembership(
            viewer_id, channel_id, membership.user_id(), membership.member_type());
    }
    for (auto const &membership : delta.to_be_removed) {
        assert(membership.channel_id() == channel_id);
        all_successful &=
            pbac->AllowDeleteMemberFromChannel(viewer_id, channel_id, membership.user_id());
    }
    for (auto const &membership : delta.to_be_added) {
        assert(membership.channel_id() == channel_id);
        all_successful &= pbac->AllowUpdateChannelMembership(
            viewer_id, channel_id, membership.user_id(), membership.member_type());
    }
    if (!all_successful) {
        return false;
    }

    // Apply the delta.
    for (auto const &membership : delta.to_be_modified) {
        UpdateMessageChannelMembership(channel_id, membership.user_id(), membership.member_type(),
                                       conns);
    }
    for (auto const &membership : delta.to_be_added) {
        all_successful &= CreateMessageChannelMembership(channel_id, membership.user_id(),
                                                         membership.member_type(), conns);
    }
    for (auto const &membership : delta.to_be_removed) {
        all_successful &= DeleteMessageChannelMembership(channel_id, membership.user_id(), conns);
    }

    return all_successful;
}

std::vector<SearchedMessageChannel> SearchMessageChannels(
    UserId const viewer_id, std::unordered_set<UserId> const &contains_member_ids,
    std::unordered_set<MessageChannelId> const &any_channel_ids,
    std::optional<std::string> const &query_text, unsigned active_member_fetch_limit,
    std::optional<Pagination> const &pagination, ConnectionReservoirInterface *conns) {
    // Build a list of required members each searched channel must contain.
    std::vector<UserId> must_have_user_ids{contains_member_ids.begin(), contains_member_ids.end()};
    if (contains_member_ids.find(viewer_id) == contains_member_ids.end()) {
        must_have_user_ids.push_back(viewer_id);
    }

    // Search message channels.
    SqlQueryBuilder message_channel_query;
    SqlQueryBuilder::Placeholder<SqlLongArr> contains_member_ids_ph;
    SqlQueryBuilder::Placeholder<SqlInt> must_have_user_count_ph;
    SqlQueryBuilder::Placeholder<SqlLong> viewer_id_ph;
    message_channel_query.QueryPiece("(SELECT mc.* FROM ")
        .QueryPiece(TableNames::MessageChannel())
        .QueryPiece(" mc")
        .QueryPiece(" JOIN ")
        .QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" required_member ON required_member.channel_id=mc.id")
        .QueryPiece(" WHERE")
        .QueryPiece(" required_member.user_id=ANY(")
        .Holder(&contains_member_ids_ph)
        .QueryPiece(")");

    if (!any_channel_ids.empty()) {
        SqlQueryBuilder::Placeholder<SqlLongArr> any_channel_ids_ph;
        message_channel_query.QueryPiece(" AND mc.id=ANY(")
            .Holder(&any_channel_ids_ph)
            .QueryPiece(")");
        message_channel_query.SetValueToPlaceholder(
            any_channel_ids_ph, std::make_shared<SqlLongArr>(std::vector<MessageChannelId>{
                                    any_channel_ids.begin(), any_channel_ids.end()}));
    }

    message_channel_query.QueryPiece(" GROUP BY mc.id HAVING COUNT(required_member.user_id)=")
        .Holder(&must_have_user_count_ph)
        .QueryPiece(") AS qualified_channel")
        .QueryPiece(" JOIN ")
        .QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" viewer ON viewer.channel_id=qualified_channel.id")
        .QueryPiece(" WHERE viewer.user_id=")
        .Holder(&viewer_id_ph)
        .QueryPiece(" ORDER BY viewer.last_interaction_at DESC");

    message_channel_query.SetValueToPlaceholder(contains_member_ids_ph,
                                                std::make_shared<SqlLongArr>(must_have_user_ids));
    message_channel_query.SetValueToPlaceholder(
        must_have_user_count_ph, std::make_shared<SqlInt>(must_have_user_ids.size()));
    message_channel_query.SetValueToPlaceholder(viewer_id_ph, std::make_shared<SqlLong>(viewer_id));

    std::vector<std::tuple<MessageChannelEntity, MessageChannelHasUserEntity>> channel_and_viewer;

    if (query_text.has_value()) {
        channel_and_viewer = Search<MessageChannelEntity, MessageChannelHasUserEntity>(
            message_channel_query, /*entity_aliases=*/{"qualified_channel", "viewer"},
            /*search_target_entity=*/{"qualified_channel"}, *query_text, /*prefix_search=*/true,
            /*rank_result=*/false, pagination->result_per_page(),
            pagination->result_per_page() * pagination->page_number(), conns);
    } else {
        if (pagination.has_value()) {
            SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
            SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
            message_channel_query.QueryPiece(" LIMIT ")
                .Holder(&limit_ph)
                .QueryPiece(" OFFSET ")
                .Holder(&offset_ph);
            message_channel_query.SetValueToPlaceholder(
                limit_ph, std::make_shared<SqlInt>(pagination->result_per_page()));
            message_channel_query.SetValueToPlaceholder(
                offset_ph, std::make_shared<SqlInt>(pagination->page_number() *
                                                    pagination->result_per_page()));
        }
        channel_and_viewer = Query<MessageChannelEntity, MessageChannelHasUserEntity>(
            message_channel_query, {"qualified_channel", "viewer"}, conns);
    }

    // Fetch the most active members for each message channel.
    std::vector<MessageChannelId> message_channel_ids(channel_and_viewer.size());
    for (unsigned i = 0; i < channel_and_viewer.size(); i++) {
        message_channel_ids[i] = *std::get<0>(channel_and_viewer[i]).id.Value();
    }
    std::vector<std::vector<MessageChannelHasUserEntity>> most_active_members =
        FetchMostActiveMembers(message_channel_ids, active_member_fetch_limit, conns);

    // Combine results.
    std::vector<SearchedMessageChannel> results(channel_and_viewer.size());
    for (unsigned i = 0; i < channel_and_viewer.size(); ++i) {
        auto const &entry = channel_and_viewer[i];

        SearchedMessageChannel result;
        result.message_channel = std::get<0>(entry);
        result.member_type =
            static_cast<MessageChannelMemberType>(*std::get<1>(entry).ownership.Value());
        result.join_at = *std::get<1>(entry).created_at.Value();
        result.most_active_members = most_active_members[i];

        results[i] = result;
    }

    return results;
}

std::vector<MessageChannelOverview>
ToMessageChannelOverviews(UserId const viewer_id,
                          std::vector<SearchedMessageChannel> const &searched_message_channels,
                          KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns) {
    // Construct member profiles.
    std::unordered_map<UserId, UserPublicProfile> active_member_profiles;
    std::vector<UserId> unique_active_member_ids;
    for (auto const &searched_channel : searched_message_channels) {
        for (MessageChannelHasUserEntity const member : searched_channel.most_active_members) {
            UserId const member_id = *member.user_id.Value();
            auto it = active_member_profiles.find(member_id);
            if (it == active_member_profiles.end()) {
                active_member_profiles[member_id] = UserPublicProfile();
                unique_active_member_ids.push_back(member_id);
            }
        }
    }

    std::vector<UserEntity> members = FetchUsers(unique_active_member_ids, conns);
    std::vector<UserPublicProfile> member_profiles =
        BuildPublicProfiles(viewer_id, members, key_gen, conns);
    for (auto const &profile : member_profiles) {
        active_member_profiles[profile.user_id()] = profile;
    }

    // Compose the overiew.
    std::vector<MessageChannelOverview> result(searched_message_channels.size());
    for (unsigned i = 0; i < searched_message_channels.size(); ++i) {
        MessageChannelOverview overview;
        *overview.mutable_channel() = GetMessageChannel(searched_message_channels[i]);
        *overview.mutable_channel_relation() =
            GetMessageChannelRelation(searched_message_channels[i]);
        overview.set_channel_last_interacted_at(searched_message_channels[i].join_at);
        for (auto const &member : searched_message_channels[i].most_active_members) {
            *overview.mutable_most_active_users()->Add() =
                active_member_profiles[*member.user_id.Value()];
        }

        result[i] = overview;
    }

    return result;
}

std::vector<MessageChannelMember>
GetMessageChannelMembers(MessageChannelId channel_id, std::optional<Pagination> const &pagination,
                         ConnectionReservoirInterface *conns) {
    SqlQueryBuilder message_channel_member_query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    message_channel_member_query.QueryPiece(TableNames::AUser())
        .QueryPiece(" u")
        .QueryPiece(" JOIN ")
        .QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" mchu ON mchu.user_id=u.id")
        .QueryPiece(" WHERE mchu.channel_id=")
        .Holder(&channel_id_ph)
        .QueryPiece(" ORDER BY mchu.last_interaction_at DESC");

    message_channel_member_query.SetValueToPlaceholder(channel_id_ph,
                                                       std::make_shared<SqlLong>(channel_id));

    if (pagination.has_value()) {
        SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
        SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
        message_channel_member_query.QueryPiece(" LIMIT ")
            .Holder(&limit_ph)
            .QueryPiece(" OFFSET ")
            .Holder(&offset_ph);
        message_channel_member_query.SetValueToPlaceholder(
            limit_ph, std::make_shared<SqlInt>(pagination->result_per_page()));
        message_channel_member_query.SetValueToPlaceholder(
            offset_ph,
            std::make_shared<SqlInt>(pagination->page_number() * pagination->result_per_page()));
    }

    std::vector<std::tuple<UserEntity, MessageChannelHasUserEntity>> query_result =
        Query<UserEntity, MessageChannelHasUserEntity>(message_channel_member_query, {"u", "mchu"},
                                                       conns);

    std::vector<MessageChannelMember> results(query_result.size());
    for (unsigned i = 0; i < query_result.size(); i++) {
        std::tuple<UserEntity, MessageChannelHasUserEntity> const &entry = query_result[i];

        MessageChannelMember result;
        result.member = std::get<0>(entry);
        result.member_type =
            static_cast<MessageChannelMemberType>(*std::get<1>(entry).ownership.Value());
        result.join_at = *std::get<1>(entry).created_at.Value();

        results[i] = result;
    }

    return results;
}

bool UserInMessageChannel(UserId const user_id, MessageChannelId const channel_id,
                          ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> channel_id_ph;
    SqlQueryBuilder::Placeholder<SqlLong> member_id_ph;
    query.QueryPiece(TableNames::MessageChannelHasUser())
        .QueryPiece(" member WHERE member.channel_id=")
        .Holder(&channel_id_ph)
        .QueryPiece(" AND member.user_id=")
        .Holder(&member_id_ph);

    query.SetValueToPlaceholder(channel_id_ph, std::make_shared<SqlLong>(channel_id));
    query.SetValueToPlaceholder(member_id_ph, std::make_shared<SqlLong>(user_id));

    return Exists(query, conns);
}

} // namespace e8
