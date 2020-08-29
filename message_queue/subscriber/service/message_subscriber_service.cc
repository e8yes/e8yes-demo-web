/**
 * e8yes demo web server.
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
#include "distributor/store/node_state_store.h"
#include "identity/auth_key.h"
#include "identity/extract_identity_from_metadata.h"
#include "identity/trustable_identity.h"
#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "message_queue/subscriber/service/message_subscriber_service.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/node.pb.h"
#include "proto_cc/service_message_queue.grpc.pb.h"
#include "proto_cc/service_message_queue.pb.h"
#include "proto_cc/service_message_subscriber.grpc.pb.h"
#include "proto_cc/service_message_subscriber.pb.h"

namespace e8 {

MessageSubscriberServiceImpl::MessageSubscriberServiceImpl(std::string const &node_state_db_path,
                                                           std::string const &key_gen_db_host,
                                                           int message_queue_port)
    : node_states_(std::make_unique<NodeStateStore>(node_state_db_path)),
      key_gen_(std::make_unique<PersistentKeyGenerator>(key_gen_db_host)),
      distributor_(std::make_unique<HashDistributor>()), message_queue_port_(message_queue_port) {}

grpc::Status MessageSubscriberServiceImpl::SubscribeRealTimeMessageQueue(
    grpc::ServerContext *context, SubscribeRealTimeMessageQueueRequest const * /*request*/,
    grpc::ServerWriter<SubscribeRealTimeMessageQueueResponse> *writer) {
    grpc::Status status;
    std::optional<Identity> identity =
        ExtractIdentityFromContext(*context, kDemoWebUserAuthorizationKey, key_gen_.get(), &status);
    if (!identity.has_value()) {
        return status;
    }

    std::optional<NodeState> node = distributor_->Distribute(std::to_string(identity->user_id()),
                                                             NDF_MESSAGE_QUEUE, node_states_.get());
    if (!node.has_value()) {
        return grpc::Status(grpc::StatusCode::UNAVAILABLE,
                            "No node is available for subscription.");
    }

    std::unique_ptr<MessageQueueService::Stub> stub =
        CREATE_GRPC_STUB(MessageQueueService, *node, message_queue_port_);

    grpc::ClientContext client_context;

    DequeueMessageRequest dequeue_request;
    dequeue_request.set_user_id(identity->user_id());

    std::unique_ptr<grpc::ClientReader<DequeueMessageResponse>> reader =
        stub->DequeueMessage(&client_context, dequeue_request);

    DequeueMessageResponse dequeue_response;
    while (reader->Read(&dequeue_response)) {
        SubscribeRealTimeMessageQueueResponse subscriber_response;
        *subscriber_response.mutable_message() = dequeue_response.message();
        writer->Write(subscriber_response);
    }

    return grpc::Status::OK;
}

} // namespace e8
