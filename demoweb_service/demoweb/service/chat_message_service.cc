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

#include <optional>

#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module/chat_message.h"
#include "demoweb_service/demoweb/module/chat_message_group.h"
#include "demoweb_service/demoweb/service/chat_message_service.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/service_chat_message.grpc.pb.h"
#include "proto_cc/service_chat_message.pb.h"

namespace e8 {

grpc::Status
ChatMessageServiceImpl::CreateChatMessageThread(grpc::ServerContext *context,
                                                CreateChatMessageThreadRequest const *request,
                                                CreateChatMessageThreadResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<ChatMessageGroupEntity> chat_mesage_group = CreateChatMessageGroup(
        identity->user_id(), request->channel_id(), request->thread_title(), request->thread_type(),
        DemoWebEnvironment()->CurrentHostId(), DemoWebEnvironment()->MessageChannelPbac(),
        DemoWebEnvironment()->DemowebDatabase());
    if (!chat_mesage_group.has_value()) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                            "You don't have enough privilege to create a chat message group in the "
                            "message channel.");
    }

    *response->mutable_thread() = ToChatMessageThread(*chat_mesage_group);

    return grpc::Status::OK;
}

grpc::Status ChatMessageServiceImpl::SendChatMessage(grpc::ServerContext *context,
                                                     SendChatMessageRequest const *request,
                                                     SendChatMessageResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<SendChatMessageResult> result = e8::SendChatMessage(
        identity->user_id(), request->thread_id(),
        std::vector<std::string>(request->texts().begin(), request->texts().end()),
        IntsToEnums<FileFormat>(request->media_file_formats()),
        IntsToEnums<FileFormat>(request->binary_file_formats()),
        DemoWebEnvironment()->MessageChannelPbac(), DemoWebEnvironment()->KeyGen(),
        DemoWebEnvironment()->DemowebDatabase());
    if (!result.has_value()) {
        return grpc::Status(grpc::StatusCode::PERMISSION_DENIED,
                            "You don't have enough privilege to send a chat message in the "
                            "message channel the chat message group belongs to.");
    }

    *response->mutable_message() = result->message;
    *response->mutable_media_file_readwrite_accesses() = {
        result->media_file_location_accesses.begin(), result->media_file_location_accesses.end()};
    *response->mutable_binary_file_readwrite_accesses() = {
        result->binary_file_location_accesses.begin(), result->binary_file_location_accesses.end()};

    return grpc::Status::OK;
}

grpc::Status ChatMessageServiceImpl::GetChatMessages(grpc::ServerContext *context,
                                                     GetChatMessagesRequest const *request,
                                                     GetChatMessagesResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::vector<ChatMessageEntry> result = e8::GetChatMessages(
        identity->user_id(), request->thread_id(), request->pagination(),
        DemoWebEnvironment()->MessageChannelPbac(), DemoWebEnvironment()->KeyGen(),
        DemoWebEnvironment()->DemowebDatabase());

    *response->mutable_messages() = {result.begin(), result.end()};

    return grpc::Status::OK;
}

grpc::Status
ChatMessageServiceImpl::GetChatMessageThreads(grpc::ServerContext *context,
                                              GetChatMessageThreadsRequest const *request,
                                              GetChatMessageThreadsResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(*context, &status);
    if (!identity.has_value()) {
        return status;
    }

    std::vector<ChatMessageThread> result = GetChatMessageGroupsWithChatMessageSummaryList(
        identity->user_id(), request->channel_id(), request->limit_per_thread(),
        request->pagination(), DemoWebEnvironment()->MessageChannelPbac(),
        DemoWebEnvironment()->KeyGen(), DemoWebEnvironment()->DemowebDatabase());

    *response->mutable_threads() = {result.begin(), result.end()};

    return grpc::Status::OK;
}

} // namespace e8
