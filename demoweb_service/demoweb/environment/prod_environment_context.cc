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
#include <vector>

#include "constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/environment/prod_environment_context.h"
#include "distributor/store/default_node_state_store.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

DemoWebProductionEnvironmentContext::DemoWebProductionEnvironmentContext(
    std::string const &db_hostname, std::string const &node_state_db_path,
    MessageQueueServicePort const message_queue_port) {
    InitDefaultNodeStateStoreProvider(node_state_db_path);

    ConnectionFactory fact(ConnectionFactory::PQ, db_hostname, kDemowebDatabaseName);
    demoweb_database_ = std::make_unique<BasicConnectionReservoir>(fact);
    SendHeartBeat(demoweb_database_.get());

    key_gen_ = std::make_unique<PersistentKeyGenerator>(db_hostname);

    host_id_ = ::e8::CurrentHostId();

    e8_message_publisher_ =
        std::make_unique<E8MessagePublisher>(DefaultNodeStateStore(), message_queue_port);
}

DemoWebEnvironmentContextInterface::Environment
DemoWebProductionEnvironmentContext::EnvironmentType() const {
    return DemoWebEnvironmentContextInterface::TEST;
}

unsigned DemoWebProductionEnvironmentContext::CurrentHostId() const { return host_id_; }

e8::ConnectionReservoirInterface *DemoWebProductionEnvironmentContext::DemowebDatabase() {
    return demoweb_database_.get();
}

e8::KeyGeneratorInterface *DemoWebProductionEnvironmentContext::KeyGen() { return key_gen_.get(); }

std::vector<MessagePublisherInterface *>
DemoWebProductionEnvironmentContext::ClientPushMessagePublishers() {
    return std::vector<MessagePublisherInterface *>{e8_message_publisher_.get()};
}

} // namespace e8
