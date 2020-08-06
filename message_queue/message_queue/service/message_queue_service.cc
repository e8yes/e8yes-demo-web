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
#include <optional>

#include "message_queue/message_queue/module/message_queue_store.h"
#include "message_queue/message_queue/service/message_queue_service.h"
#include "proto_cc/real_time_message.pb.h"
#include "proto_cc/service_message_queue.grpc.pb.h"
#include "proto_cc/service_message_queue.pb.h"

namespace e8 {

grpc::Status MessageQueueServiceImpl::EnqueueMessage(grpc::ServerContext * /*context*/,
                                                     EnqueueMessageRequest const *request,
                                                     EnqueueMessageResponse * /*response*/) {
    for (auto const &message : request->messages()) {
        MessageQueueStoreInstance()->Enqueue(request->user_id(), message);
    }
    return grpc::Status::OK;
}

grpc::Status
MessageQueueServiceImpl::DequeueMessage(grpc::ServerContext * /*context*/,
                                        DequeueMessageRequest const *request,
                                        grpc::ServerWriter<DequeueMessageResponse> *writer) {
    while (true) {
        RealTimeMessage message = MessageQueueStoreInstance()->BlockingDequeue(request->user_id());

        DequeueMessageResponse res;
        *res.mutable_message() = message;

        writer->Write(res);
    }
    return grpc::Status::OK;
}

} // namespace e8
