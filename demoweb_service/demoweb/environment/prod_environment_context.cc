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
#include "demoweb_service/demoweb/environment/prod_environment_context.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

ProductionEnvironmentContext::ProductionEnvironmentContext(std::string const &demoweb_db_hostname,
                                                           int demoweb_db_port,
                                                           std::string const &demoweb_db_user,
                                                           std::string const &demoweb_db_password) {
    ConnectionFactory fact(ConnectionFactory::PQ, demoweb_db_hostname, demoweb_db_port,
                           kDemowebDatabaseName, demoweb_db_user, demoweb_db_password);
    demoweb_database_ = std::make_unique<BasicConnectionReservoir>(fact);
    SendHeartBeat(demoweb_database_.get());

    key_gen_ = std::make_unique<PersistentKeyGenerator>(demoweb_database_.get());

    host_id_ = CurrentHostId();
}

EnvironmentContextInterface::Environment ProductionEnvironmentContext::EnvironmentType() const {
    return EnvironmentContextInterface::TEST;
}

unsigned ProductionEnvironmentContext::CurrentHostId() const { return host_id_; }

e8::ConnectionReservoirInterface *ProductionEnvironmentContext::DemowebDatabase() {
    return demoweb_database_.get();
}

e8::KeyGeneratorInterface *ProductionEnvironmentContext::KeyGen() { return key_gen_.get(); }

} // namespace e8
