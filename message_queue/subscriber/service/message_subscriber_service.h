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

#ifndef SUBSCRIBERSERVICEIMPL_H
#define SUBSCRIBERSERVICEIMPL_H

#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "keygen/key_generator_interface.h"
#include "proto_cc/service_message_subscriber.grpc.pb.h"
#include "proto_cc/service_message_subscriber.pb.h"

namespace e8 {

/**
 * @brief The SubscriberServiceImpl class Service for handling message queue subscriptions.
 */
class MessageSubscriberServiceImpl : public MessageSubscriberService::Service {
  public:
    MessageSubscriberServiceImpl(std::string const &node_state_db_path, std::string const &db_host,
                                 int db_port, std::string const &db_user,
                                 std::string const &db_password, int message_queue_port);
    ~MessageSubscriberServiceImpl() override = default;

    grpc::Status SubscribeRealTimeMessageQueue(
        grpc::ServerContext *context, SubscribeRealTimeMessageQueueRequest const *request,
        grpc::ServerWriter<SubscribeRealTimeMessageQueueResponse> *writer) override;

  private:
    std::unique_ptr<NodeStateStoreInterface> node_states_;
    std::unique_ptr<KeyGeneratorInterface> key_gen_;
    std::unique_ptr<DistributorInterface> distributor_;
    int const message_queue_port_;
};

} // namespace e8

#endif // SUBSCRIBERSERVICEIMPL_H
