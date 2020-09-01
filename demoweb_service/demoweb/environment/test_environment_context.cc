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
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

DemoWebTestEnvironmentContext::DemoWebTestEnvironmentContext() {
    ConnectionFactory fact(ConnectionFactory::PQ, /*host_name=*/"localhost", kDemowebDatabaseName);
    demoweb_database_ = std::make_unique<BasicConnectionReservoir>(fact);
    SendHeartBeat(demoweb_database_.get());
    ClearAllTables(demoweb_database_.get());

    key_gen_ = std::make_unique<PersistentKeyGenerator>(/*host_name=*/"localhost");

    host_id_ = 0;
}

DemoWebEnvironmentContextInterface::Environment
DemoWebTestEnvironmentContext::EnvironmentType() const {
    return DemoWebEnvironmentContextInterface::TEST;
}

HostId DemoWebTestEnvironmentContext::CurrentHostId() const { return host_id_; }

ConnectionReservoirInterface *DemoWebTestEnvironmentContext::DemowebDatabase() {
    return demoweb_database_.get();
}

KeyGeneratorInterface *DemoWebTestEnvironmentContext::KeyGen() { return key_gen_.get(); }

std::vector<MessagePublisherInterface *>
DemoWebTestEnvironmentContext::ClientPushMessagePublishers() {
    return std::vector<MessagePublisherInterface *>();
}

} // namespace e8
