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

#include <grpcpp/grpcpp.h>
#include <memory>
#include <optional>
#include <string>

#include "distributor/distributor/distribute.h"
#include "distributor/distributor/grpc_stub.h"
#include "identity/auth_key.h"
#include "identity/extract_identity_from_metadata.h"
#include "identity/trustable_identity.h"
#include "message_queue/subscriber/environment/environment_context_interface.h"
#include "message_queue/subscriber/service/message_subscriber_service.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/node.pb.h"
#include "proto_cc/service_message_queue.grpc.pb.h"
#include "proto_cc/service_message_queue.pb.h"
#include "proto_cc/service_message_subscriber.grpc.pb.h"
#include "proto_cc/service_message_subscriber.pb.h"

namespace e8 {

// TODO: Make this function testable.
grpc::Status MessageSubscriberServiceImpl::SubscribeRealTimeMessageQueue(
    grpc::ServerContext *context, SubscribeRealTimeMessageQueueRequest const * /*request*/,
    grpc::ServerWriter<SubscribeRealTimeMessageQueueResponse> *writer) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(
        *context, kDemoWebUserAuthorizationKey, SubscriberEnvironment()->KeyGen(), &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<NodeState> node = SubscriberEnvironment()->Distributor()->Distribute(
        std::to_string(identity->user_id()), NDF_MESSAGE_QUEUE,
        SubscriberEnvironment()->NodeStateStorage());
    if (!node.has_value()) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE,
                            "No node is available for subscription.");
    }

    std::unique_ptr<MessageQueueService::Stub> stub = CREATE_GRPC_STUB(
        MessageQueueService, *node, SubscriberEnvironment()->GetMessageQueueServicePort());

    grpc::ClientContext client_context;

    std::unique_ptr<grpc::ClientReaderWriter<DequeueMessageRequest, DequeueMessageResponse>>
        stream = stub->DequeueMessage(&client_context);

    DequeueMessageRequest dequeue_request;
    dequeue_request.set_user_id(identity->user_id());
    dequeue_request.set_previous_message_delivered(false);
    dequeue_request.set_end_operation(false);

    DequeueMessageResponse dequeue_response;
    while (stream->Write(dequeue_request) && stream->Read(&dequeue_response)) {
        SubscribeRealTimeMessageQueueResponse subscriber_response;
        *subscriber_response.mutable_message() = dequeue_response.message();
        if (writer->Write(subscriber_response)) {
            dequeue_request.set_previous_message_delivered(true);
        } else {
            dequeue_request.set_previous_message_delivered(false);
            dequeue_request.set_end_operation(true);
        }
    }

    return grpc::Status::OK;
}

grpc::Status MessageSubscriberServiceImpl::SubscribeRealTimeMessageQueueLP(
    grpc::ServerContext *context, SubscribeRealTimeMessageQueueRequest const * /*request*/,
    SubscribeRealTimeMessageQueueResponse *response) {
    grpc::Status status;
    std::optional<Identity> identity = ExtractIdentityFromContext(
        *context, kDemoWebUserAuthorizationKey, SubscriberEnvironment()->KeyGen(), &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<NodeState> node = SubscriberEnvironment()->Distributor()->Distribute(
        std::to_string(identity->user_id()), NDF_MESSAGE_QUEUE,
        SubscriberEnvironment()->NodeStateStorage());
    if (!node.has_value()) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE,
                            "No node is available for subscription.");
    }

    std::unique_ptr<MessageQueueService::Stub> stub = CREATE_GRPC_STUB(
        MessageQueueService, *node, SubscriberEnvironment()->GetMessageQueueServicePort());

    grpc::ClientContext client_context;

    std::unique_ptr<grpc::ClientReaderWriter<DequeueMessageRequest, DequeueMessageResponse>>
        stream = stub->DequeueMessage(&client_context);

    DequeueMessageRequest dequeue_request;
    dequeue_request.set_user_id(identity->user_id());
    dequeue_request.set_previous_message_delivered(false);
    dequeue_request.set_end_operation(false);

    std::time_t begin;
    std::time_t end;
    std::time(&begin);
    DequeueMessageResponse dequeue_response;
    if (!stream->Write(dequeue_request) || !stream->Read(&dequeue_response)) {
        return grpc::Status(grpc::StatusCode::INTERNAL, "Message queue stream is broken.");
    }
    std::time(&end);

    if (end - begin > 30) {
        *response->mutable_message() = dequeue_response.message();
        dequeue_request.set_previous_message_delivered(true);
    } else {
        dequeue_request.set_previous_message_delivered(false);
    }

    dequeue_request.set_end_operation(true);
    stream->Write(dequeue_request);

    return grpc::Status::OK;
}

} // namespace e8
