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
#include "distributor/store/node_state_store.h"
#include "message_queue/common/entity.h"
#include "message_queue/common/message_queue_distributor.h"
#include "message_queue/publisher/publisher.h"
#include "proto_cc/node.pb.h"
#include "proto_cc/real_time_message.pb.h"
#include "proto_cc/service_message_queue.grpc.pb.h"
#include "proto_cc/service_message_queue.pb.h"

namespace e8 {

E8MessagePublisher::E8MessagePublisher(std::shared_ptr<NodeStateStoreInterface> const &node_states,
                                       MessageQueueServicePort const message_queue_service_port)
    : distributor_(CreateMessageQueueDistributor()), node_states_(node_states),
      message_queue_service_port_(message_queue_service_port) {}

bool E8MessagePublisher::Publish(MessageKey key, RealTimeMessage const &message) {
    std::optional<NodeState> node =
        distributor_->Distribute(std::to_string(key), NDF_MESSAGE_QUEUE, node_states_.get());
    if (!node.has_value()) {
        return false;
    }

    std::unique_ptr<MessageQueueService::Stub> stub =
        CREATE_GRPC_STUB(MessageQueueService, *node, message_queue_service_port_);

    grpc::ClientContext context;

    EnqueueMessageRequest request;
    request.set_user_id(key);
    *request.mutable_messages()->Add() = message;

    EnqueueMessageResponse response;
    grpc::Status status = stub->EnqueueMessage(&context, request, &response);
    if (!status.ok()) {
        return false;
    }

    return true;
}

} // namespace e8
