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

#ifndef SUBSCRIBER_PROD_ENVIRONMENT_CONTEXT_H
#define SUBSCRIBER_PROD_ENVIRONMENT_CONTEXT_H

#include <memory>
#include <string>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "keygen/key_generator_interface.h"
#include "message_queue/subscriber/environment/environment_context_interface.h"

namespace e8 {

/**
 * @brief The SubscriberProductionEnvironmentContext class Manages global objects for the production
 * deployment environment.
 */
class SubscriberProductionEnvironmentContext : public SubscriberEnvironmentContextInterface {
  public:
    SubscriberProductionEnvironmentContext(std::string const &node_state_db_path,
                                           std::string const &key_gen_db_host,
                                           MessageQueueServicePort message_queue_port);
    ~SubscriberProductionEnvironmentContext() override = default;

    Environment EnvironmentType() const override;

    KeyGeneratorInterface *KeyGen() override;

    NodeStateStoreInterface *NodeStateStorage() override;

    DistributorInterface *Distributor() override;

    MessageQueueServicePort GetMessageQueueServicePort() override;

  private:
    std::unique_ptr<NodeStateStoreInterface> node_states_;
    std::unique_ptr<KeyGeneratorInterface> key_gen_;
    std::unique_ptr<DistributorInterface> distributor_;
    MessageQueueServicePort const message_queue_port_;
};

} // namespace e8

#endif // SUBSCRIBER_PROD_ENVIRONMENT_CONTEXT_H
