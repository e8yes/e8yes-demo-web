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

#include <memory>

#include "distributor/distributor/distribute.h"
#include "distributor/store/node_state_store.h"
#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "message_queue/subscriber/environment/environment_context_interface.h"
#include "message_queue/subscriber/environment/test_environment_context.h"

namespace e8 {

SubscriberTestEnvironmentContext::SubscriberTestEnvironmentContext()
    : node_states_(std::make_unique<NodeStateStore>("node_states_test.sqlite")),
      key_gen_(std::make_unique<PersistentKeyGenerator>("localhost")),
      distributor_(std::make_unique<HashDistributor>()) {}

SubscriberTestEnvironmentContext::Environment
SubscriberTestEnvironmentContext::EnvironmentType() const {
    return Environment::TEST;
}

KeyGeneratorInterface *SubscriberTestEnvironmentContext::KeyGen() { return key_gen_.get(); }

NodeStateStoreInterface *SubscriberTestEnvironmentContext::NodeStateStorage() {
    return node_states_.get();
}

DistributorInterface *SubscriberTestEnvironmentContext::Distributor() { return distributor_.get(); }

MessageQueueServicePort SubscriberTestEnvironmentContext::GetMessageQueueServicePort() { return 0; }

} // namespace e8
