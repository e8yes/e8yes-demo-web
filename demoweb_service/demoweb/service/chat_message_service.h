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

#ifndef CHAT_MESSAGE_SERVICE_IMPL_H
#define CHAT_MESSAGE_SERVICE_IMPL_H

#include "proto_cc/chat_message.pb.h"
#include "proto_cc/service_chat_message.grpc.pb.h"
#include "proto_cc/service_chat_message.pb.h"

namespace e8 {

/**
 * @brief The ChatMessageServiceImpl class ervice for chat message management.
 */
class ChatMessageServiceImpl : public ChatMessageService::Service {
  public:
    ChatMessageServiceImpl() = default;
    ~ChatMessageServiceImpl() override = default;

    grpc::Status CreateChatMessageThread(grpc::ServerContext *context,
                                         CreateChatMessageThreadRequest const *request,
                                         CreateChatMessageThreadResponse *response) override;

    grpc::Status SendChatMessage(grpc::ServerContext *context,
                                 SendChatMessageRequest const *request,
                                 SendChatMessageResponse *response) override;

    grpc::Status GetChatMessages(grpc::ServerContext *context,
                                 GetChatMessagesRequest const *request,
                                 GetChatMessagesResponse *response) override;

    grpc::Status GetChatMessageThreads(grpc::ServerContext *context,
                                       GetChatMessageThreadsRequest const *request,
                                       GetChatMessageThreadsResponse *response) override;
};

} // namespace e8

#endif // CHAT_MESSAGE_SERVICE_IMPL_H
