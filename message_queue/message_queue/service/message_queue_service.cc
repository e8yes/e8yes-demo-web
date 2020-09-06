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
#include <optional>

#include "message_queue/message_queue/module/message_queue_store.h"
#include "message_queue/message_queue/service/message_queue_service.h"
#include "proto_cc/real_time_message.pb.h"
#include "proto_cc/service_message_queue.grpc.pb.h"
#include "proto_cc/service_message_queue.pb.h"

namespace e8 {
namespace {

grpc::Status
WriteToStream(RealTimeMessage const &message,
              grpc::ServerReaderWriter<DequeueMessageResponse, DequeueMessageRequest> *stream) {
    DequeueMessageResponse res;
    *res.mutable_message() = message;

    bool successful = stream->Write(res);
    if (!successful) {
        return grpc::Status(grpc::StatusCode::ABORTED, "Stream closed.");
    }

    return grpc::Status::OK;
}

} // namespace

grpc::Status MessageQueueServiceImpl::EnqueueMessage(grpc::ServerContext * /*context*/,
                                                     EnqueueMessageRequest const *request,
                                                     EnqueueMessageResponse * /*response*/) {
    for (auto const &message : request->messages()) {
        MessageQueueStoreInstance()->Enqueue(request->user_id(), message);
    }
    return grpc::Status::OK;
}

// TODO: Make this functiont testable.
// TODO: Add the option to abort the dequeue operation after certain amount of inactivity.
grpc::Status MessageQueueServiceImpl::DequeueMessage(
    grpc::ServerContext * /*context*/,
    grpc::ServerReaderWriter<DequeueMessageResponse, DequeueMessageRequest> *stream) {
    MessageKey user_id = 0;
    RealTimeMessage message;
    MessageQueueStore::MessageQueue *queue = nullptr;
    grpc::Status current_status = grpc::Status::OK;

    while (current_status.ok()) {
        DequeueMessageRequest request;
        if (!stream->Read(&request)) {
            current_status = grpc::Status(grpc::StatusCode::ABORTED, "Stream closed.");
            continue;
        }

        // Guarantees that the user_id is consistent over the request stream.
        if (user_id != 0 && user_id != request.user_id()) {
            current_status = grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                                          "Can't operate on different queues.");
            continue;
        }
        user_id = request.user_id();
        assert(user_id != 0);

        // Remove the oldest element only when the client successfully delivered the message.
        if (queue != nullptr) {
            MessageQueueStoreInstance()->EndBlockingDequeue(
                queue, /*dequeue=*/request.previous_message_delivered());
        }

        if (request.end_operation()) {
            current_status = grpc::Status(grpc::StatusCode::ABORTED,
                                          "Client asks to halt the dequeue operation.");
            continue;
        }

        queue = MessageQueueStoreInstance()->BeginBlockingDequeue(
            user_id, request.wait_duration_secs(), &message);

        if (queue == nullptr) {
            current_status = grpc::Status(grpc::StatusCode::ABORTED, "Time out.");
            continue;
        }

        current_status = WriteToStream(message, stream);
    }

    // Put the message back in any failure cases.
    if (!current_status.ok() && queue != nullptr) {
        MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/false);
    }

    return current_status;
}

} // namespace e8
