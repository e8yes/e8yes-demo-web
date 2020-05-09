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

#include <memory>

#include "constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

TestEnvironmentContext::TestEnvironmentContext() {
    ConnectionFactory fact(/*connection_type=*/ConnectionFactory::PQ, /*host_name=*/"localhost",
                           /*port=*/5432,
                           /*db_name=*/kDemowebDatabaseName, /*user_name*/ "postgres",
                           /*password=*/"password");
    demoweb_database_ = std::make_unique<BasicConnectionReservoir>(fact);
    SendHeartBeat(demoweb_database_.get());
    ClearAllTables(demoweb_database_.get());

    key_gen_ = std::make_unique<PersistentKeyGenerator>(demoweb_database_.get());

    host_id_ = CurrentHostId();
}

EnvironmentContextInterface::Environment TestEnvironmentContext::EnvironmentType() const {
    return EnvironmentContextInterface::TEST;
}

unsigned TestEnvironmentContext::CurrentHostId() const { return host_id_; }

e8::ConnectionReservoirInterface *TestEnvironmentContext::DemowebDatabase() {
    return demoweb_database_.get();
}

e8::KeyGeneratorInterface *TestEnvironmentContext::KeyGen() { return key_gen_.get(); }

} // namespace e8
