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

#ifndef SUBSCRIBERSERVICEIMPL_H
#define SUBSCRIBERSERVICEIMPL_H

#include <grpcpp/grpcpp.h>

#include "proto_cc/service_message_subscriber.grpc.pb.h"
#include "proto_cc/service_message_subscriber.pb.h"

namespace e8 {

/**
 * @brief The SubscriberServiceImpl class Service for handling message queue subscriptions.
 */
class MessageSubscriberServiceImpl : public MessageSubscriberService::Service {
  public:
    MessageSubscriberServiceImpl() = default;
    ~MessageSubscriberServiceImpl() override = default;

    grpc::Status SubscribeRealTimeMessageQueue(
        grpc::ServerContext *context, SubscribeRealTimeMessageQueueRequest const *request,
        grpc::ServerWriter<SubscribeRealTimeMessageQueueResponse> *writer) override;
};

} // namespace e8

#endif // SUBSCRIBERSERVICEIMPL_H
