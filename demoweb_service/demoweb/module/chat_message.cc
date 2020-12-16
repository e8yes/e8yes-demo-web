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
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/chat_message.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/chat_message_storage.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/file.pb.h"

namespace e8 {
namespace {

std::vector<ChatMessageEntry>
ToChatMessageEntries(std::vector<std::tuple<ChatMessageEntity, UserEntity>> const &entities,
                     KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns) {
    std::unordered_set<UserId> unique_sender_ids;
    std::vector<UserEntity> unqiue_senders;
    for (auto const &[_, sender] : entities) {
        auto it = unique_sender_ids.find(*sender.id.Value());
        if (it == unique_sender_ids.end()) {
            unique_sender_ids.insert(*sender.id.Value());
            unqiue_senders.push_back(sender);
        }
    }

    std::vector<UserPublicProfile> sender_profiles =
        BuildPublicProfiles(/*viewer_id=*/std::nullopt, unqiue_senders, key_gen, conns);
    std::unordered_map<UserId, UserPublicProfile const *> sender_profile_lookup(
        sender_profiles.size());
    for (auto const &sender_profile : sender_profiles) {
        sender_profile_lookup.insert(std::make_pair(sender_profile.user_id(), &sender_profile));
    }

    std::vector<ChatMessageEntry> entries(entities.size());
    for (unsigned i = 0; i < entries.size(); ++i) {
        auto const &[chat_message, _] = entities[i];
        entries[i].set_thread_id(*chat_message.group_id.Value());
        entries[i].set_message_seq_id(*chat_message.message_seq_id.Value());
        *entries[i].mutable_sender() = *sender_profile_lookup[*chat_message.sender_id.Value()];
        entries[i].set_created_at(*chat_message.created_at.Value());
        *entries[i].mutable_texts() = {chat_message.text_entries.Value().begin(),
                                       chat_message.text_entries.Value().end()};
        // TODO: manages media and binary file accesses.
    }

    return entries;
}

} // namespace

std::optional<SendChatMessageResult> SendChatMessage(
    UserId const sender_id, ChatMessageGroupId const group_id,
    std::vector<std::string> const &texts, std::vector<FileFormat> const & /*media_file_formats*/,
    std::vector<FileFormat> const & /*binary_file_formats*/, MessageChannelPbacInterface *pbac,
    KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns) {
    std::optional<ChatMessageGroupEntity> group = FetchChatMessageGroup(group_id, conns);
    if (!group.has_value()) {
        return std::nullopt;
    }
    if (!pbac->AllowSendChatMessage(sender_id, *group->channel_id.Value())) {
        return std::nullopt;
    }

    ChatMessageEntity entity =
        CreateChatMessage(group_id, sender_id, texts,
                          /*binary_content_paths=*/std::vector<std::string>(), conns);

    switch (*group->group_type.Value()) {
    case CMTT_POPUP: {
        *group->last_interaction_at.ValuePtr() = *entity.created_at.Value();
        int64_t rows = Update(*group, TableNames::ChatMessageGroup(), /*replace=*/true, conns);
        assert(rows == 1);
        break;
    }
    case CMTT_TEMPORAL: {
        // Do nothing.
        break;
    }
    }

    SendChatMessageResult result;
    std::optional<UserEntity> sender = FetchUser(sender_id, conns);
    assert(sender.has_value());
    result.message = ToChatMessageEntries(
        std::vector<std::tuple<ChatMessageEntity, UserEntity>>{std::make_tuple(entity, *sender)},
        key_gen, conns)[0];

    return result;
}

std::vector<ChatMessageEntry>
GetChatMessages(UserId const viewer_id, ChatMessageGroupId const group_id,
                std::optional<Pagination> const &pagination, MessageChannelPbacInterface *pbac,
                KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns) {
    std::optional<ChatMessageGroupEntity> group = FetchChatMessageGroup(group_id, conns);
    if (!group.has_value()) {
        return std::vector<ChatMessageEntry>();
    }
    if (!pbac->AllowReadChatMessageGroup(viewer_id, *group->channel_id.Value())) {
        return std::vector<ChatMessageEntry>();
    }

    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> group_id_ph;
    query.QueryPiece(TableNames::ChatMessage())
        .QueryPiece(" cm JOIN ")
        .QueryPiece(TableNames::AUser())
        .QueryPiece(" sender ON sender.id = cm.sender_id WHERE cm.group_id=")
        .Holder(&group_id_ph)
        .QueryPiece(" ORDER BY cm.message_seq_id ASC");

    if (pagination.has_value()) {
        SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
        SqlQueryBuilder::Placeholder<SqlInt> offset_ph;
        query.QueryPiece(" LIMIT ").Holder(&limit_ph);
        query.QueryPiece(" OFFSET ").Holder(&offset_ph);

        query.SetValueToPlaceholder(limit_ph,
                                    std::make_shared<SqlInt>(pagination->result_per_page()));
        query.SetValueToPlaceholder(
            offset_ph,
            std::make_shared<SqlInt>(pagination->page_number() * pagination->result_per_page()));
    }

    query.SetValueToPlaceholder(group_id_ph, std::make_shared<SqlLong>(group_id));

    std::vector<std::tuple<ChatMessageEntity, UserEntity>> query_results =
        Query<ChatMessageEntity, UserEntity>(query, {"cm", "sender"}, conns);

    return ToChatMessageEntries(query_results, key_gen, conns);
}

} // namespace e8
