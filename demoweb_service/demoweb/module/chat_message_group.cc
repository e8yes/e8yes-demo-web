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
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/chat_message_entity.h"
#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/chat_message_group.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/chat_message.pb.h"

namespace e8 {
namespace {
std::vector<std::tuple<ChatMessageThread, std::optional<ChatMessageEntry>>> ToChatMessageEntries(
    std::vector<std::tuple<ChatMessageGroupEntity, ChatMessageEntity, UserEntity>> const &entities,
    KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns) {
    std::unordered_set<UserId> unique_sender_ids;
    std::vector<UserEntity> unqiue_senders;
    for (auto const &[_, chat_message, sender] : entities) {
        if (!sender.id.Value().has_value()) {
            continue;
        }

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

    std::vector<std::tuple<ChatMessageThread, std::optional<ChatMessageEntry>>> entries(
        entities.size());
    for (unsigned i = 0; i < entities.size(); ++i) {
        auto const &[chat_message_group, chat_message, sender] = entities[i];

        ChatMessageThread chat_message_thread = ToChatMessageThread(chat_message_group);

        std::optional<ChatMessageEntry> message_entry;
        if (chat_message.message_seq_id.Value().has_value()) {
            ChatMessageEntry entry;

            entry.set_thread_id(*chat_message_group.id.Value());
            entry.set_message_seq_id(*chat_message.message_seq_id.Value());
            *entry.mutable_texts() = {chat_message.text_entries.Value().begin(),
                                      chat_message.text_entries.Value().end()};
            entry.set_created_at(*chat_message.created_at.Value());

            auto sender_it = sender_profile_lookup.find(*chat_message.sender_id.Value());
            assert(sender_it != sender_profile_lookup.end());
            assert(sender_it->second != nullptr);
            *entry.mutable_sender() = *sender_it->second;

            message_entry = entry;
        }

        entries[i] = std::make_tuple(chat_message_thread, message_entry);
    }

    return entries;
}

std::vector<ChatMessageThread> GroupByMessageGroup(
    std::vector<std::tuple<ChatMessageThread, std::optional<ChatMessageEntry>>> const
        &chat_message_entries) {
    std::vector<ChatMessageThread> result;
    std::unordered_map<ChatMessageGroupId, ChatMessageThread *> chat_message_thread_lookup;

    for (auto const &[chat_message_thread, chat_message] : chat_message_entries) {
        auto it = chat_message_thread_lookup.find(chat_message_thread.thread_id());
        if (it == chat_message_thread_lookup.end()) {
            result.push_back(chat_message_thread);

            it = chat_message_thread_lookup
                     .insert(std::make_pair(chat_message_thread.thread_id(), &result.back()))
                     .first;
        }

        if (chat_message.has_value()) {
            ChatMessageThread *message_thread = it->second;
            *message_thread->mutable_messages()->Add() = *chat_message;
        }
    }

    return result;
}

} // namespace

ChatMessageThread ToChatMessageThread(ChatMessageGroupEntity const &entity) {
    ChatMessageThread chat_message_thread;

    chat_message_thread.set_thread_id(*entity.id.Value());
    chat_message_thread.set_channel_id(*entity.channel_id.Value());
    chat_message_thread.set_thread_type(
        static_cast<ChatMessageThreadType>(*entity.group_type.Value()));
    chat_message_thread.set_thread_title(*entity.description.Value());
    chat_message_thread.set_created_at(*entity.created_at.Value());
    chat_message_thread.set_last_interaction_at(*entity.last_interaction_at.Value());

    return chat_message_thread;
}

std::optional<ChatMessageGroupEntity>
CreateChatMessageGroup(UserId const creator_id, MessageChannelId const channel_id,
                       std::string const &group_title, ChatMessageThreadType const thread_type,
                       HostId const host_id, MessageChannelPbacInterface *pbac,
                       ConnectionReservoirInterface *conns) {
    if (!pbac->AllowCreateChatMessageGroup(creator_id, channel_id)) {
        return std::nullopt;
    }
    return CreateChatMessageGroup(creator_id, channel_id, group_title, thread_type, host_id, conns);
}

std::vector<ChatMessageThread> GetChatMessageGroupsWithChatMessageSummaryList(
    UserId const viewer_id, MessageChannelId const channel_id,
    int32_t const max_num_messages_per_group, Pagination const pagination,
    MessageChannelPbacInterface *pbac, KeyGeneratorInterface *key_gen,
    ConnectionReservoirInterface *conns) {
    if (!pbac->AllowReadChatMessageGroup(viewer_id, channel_id)) {
        return std::vector<ChatMessageThread>();
    }

    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> message_channel_id_ph;
    SqlQueryBuilder::Placeholder<SqlInt> chat_message_group_limit_ph;
    SqlQueryBuilder::Placeholder<SqlInt> chat_message_group_offset_ph;
    SqlQueryBuilder::Placeholder<SqlInt> max_num_messages_per_group_ph;
    query.QueryPiece("(SELECT * FROM ")
        .QueryPiece(TableNames::ChatMessageGroup())
        .QueryPiece(" cmg")
        .QueryPiece(" WHERE cmg.channel_id=")
        .Holder(&message_channel_id_ph)
        .QueryPiece(" ORDER BY cmg.last_interaction_at ASC"
                    " LIMIT ")
        .Holder(&chat_message_group_limit_ph)
        .QueryPiece(" OFFSET ")
        .Holder(&chat_message_group_offset_ph)
        .QueryPiece(")AS paginated_cmg")
        .QueryPiece(" LEFT JOIN ")
        .QueryPiece(TableNames::ChatMessage())
        .QueryPiece(" cm ON cm.group_id=paginated_cmg.id")
        .QueryPiece(" LEFT JOIN ")
        .QueryPiece(TableNames::AUser())
        .QueryPiece(" sender ON sender.id=cm.sender_id")
        .QueryPiece(" WHERE"
                    " cm IS NULL OR"
                    " cm.message_seq_id >= ("
                    "SELECT MIN(valid_messages.message_seq_id)"
                    "FROM"
                    "(SELECT valid_cm.message_seq_id"
                    " FROM chat_message valid_cm"
                    " WHERE valid_cm.group_id = paginated_cmg.id"
                    " ORDER BY valid_cm.message_seq_id DESC"
                    " LIMIT ")
        .Holder(&max_num_messages_per_group_ph)
        .QueryPiece(")AS valid_messages)")
        .QueryPiece("ORDER BY paginated_cmg.last_interaction_at ASC, cm.message_seq_id ASC");

    query.SetValueToPlaceholder(message_channel_id_ph, std::make_shared<SqlLong>(channel_id));
    query.SetValueToPlaceholder(chat_message_group_limit_ph,
                                std::make_shared<SqlInt>(pagination.result_per_page()));
    query.SetValueToPlaceholder(
        chat_message_group_offset_ph,
        std::make_shared<SqlInt>(pagination.page_number() * pagination.result_per_page()));
    query.SetValueToPlaceholder(max_num_messages_per_group_ph,
                                std::make_shared<SqlInt>(max_num_messages_per_group));

    std::vector<std::tuple<ChatMessageGroupEntity, ChatMessageEntity, UserEntity>> query_result =
        Query<ChatMessageGroupEntity, ChatMessageEntity, UserEntity>(
            query, {"paginated_cmg", "cm", "sender"}, conns);

    std::vector<std::tuple<ChatMessageThread, std::optional<ChatMessageEntry>>>
        chat_message_entries = ToChatMessageEntries(query_result, key_gen, conns);

    return GroupByMessageGroup(chat_message_entries);
}

} // namespace e8
