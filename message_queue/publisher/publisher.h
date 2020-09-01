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

#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <memory>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "message_queue/common/entity.h"
#include "proto_cc/real_time_message.pb.h"

namespace e8 {

/**
 * @brief The MessagePublisherInterface class Provides a way to send keyed messages to a message
 * queue, which can later be pushed to the client.
 */
class MessagePublisherInterface {
  public:
    MessagePublisherInterface() = default;
    virtual ~MessagePublisherInterface() = default;

    virtual bool Publish(MessageKey key, RealTimeMessage const &message) = 0;
};

/**
 * @brief The E8MessagePublisher class Pushes keyed messages to the internal distributed message
 * queue.
 *
 * TODO: Test this implementation.
 */
class E8MessagePublisher : public MessagePublisherInterface {
  public:
    E8MessagePublisher(std::shared_ptr<NodeStateStore> const &node_states,
                       MessageQueueServicePort const message_queue_service_port);
    ~E8MessagePublisher() override = default;

    bool Publish(MessageKey key, RealTimeMessage const &message) override;

  private:
    std::unique_ptr<DistributorInterface> distributor_;
    std::shared_ptr<NodeStateStore> node_states_;
    MessageQueueServicePort const message_queue_service_port_;
};

} // namespace e8

#endif // PUBLISHER_H
