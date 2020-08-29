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

#ifndef SUBSCRIBER_ENVIRONMENT_CONTEXT_INTERFACE_H
#define SUBSCRIBER_ENVIRONMENT_CONTEXT_INTERFACE_H

#include <cstdint>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "keygen/key_generator_interface.h"

namespace e8 {

using MessageQueueServicePort = uint16_t;

/**
 * @brief The EnvironmentContextInterface class Returns global objects the runs in a deployment
 * environment.
 */
class SubscriberEnvironmentContextInterface {
  public:
    enum Environment { PROD, TEST };

    SubscriberEnvironmentContextInterface() = default;
    SubscriberEnvironmentContextInterface(SubscriberEnvironmentContextInterface const &) = delete;
    virtual ~SubscriberEnvironmentContextInterface() = default;

    /**
     * The type of deployment environment it implements.
     *
     * @return Environment type.
     */
    virtual Environment EnvironmentType() const = 0;

    /**
     * @brief KeyGen Cryptographic key generator.
     */
    virtual KeyGeneratorInterface *KeyGen() = 0;

    /**
     * @brief NodeStateStore Local node state persistent store.
     */
    virtual NodeStateStoreInterface *NodeStateStorage() = 0;

    /**
     * @brief Distributor Keyed data object distributor.
     */
    virtual DistributorInterface *Distributor() = 0;

    /**
     * @brief GetMessageQueuePort Get the port number that allows communication with the message
     * queue service.
     */
    virtual MessageQueueServicePort GetMessageQueueServicePort() = 0;
};

/**
 * @brief RegisterEnvironment Register the specified environment to be the currently active
 * environment.
 */
void RegisterEnvironment(SubscriberEnvironmentContextInterface *env);

/**
 * @brief SubscriberEnvironment Retrieve the currently registered environment context. The returned
 * context is guaranteed to be not null. If no environment is registered, this function will fail.
 */
SubscriberEnvironmentContextInterface *SubscriberEnvironment();

} // namespace e8

#endif // SUBSCRIBER_ENVIRONMENT_CONTEXT_INTERFACE_H
