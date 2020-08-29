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

#ifndef PROD_ENVIRONMENT_CONTEXT_H
#define PROD_ENVIRONMENT_CONTEXT_H

#include <cstdint>
#include <memory>
#include <string>

#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief The DemoWebProductionEnvironmentContext class Manages global objects for the production
 * deployment environment.
 */
class DemoWebProductionEnvironmentContext : public DemoWebEnvironmentContextInterface {
  public:
    DemoWebProductionEnvironmentContext(std::string const &demoweb_db_hostname);
    ~DemoWebProductionEnvironmentContext() override = default;

    Environment EnvironmentType() const override;
    HostId CurrentHostId() const override;
    e8::ConnectionReservoirInterface *DemowebDatabase() override;
    e8::KeyGeneratorInterface *KeyGen() override;

  private:
    std::unique_ptr<ConnectionReservoirInterface> demoweb_database_;
    std::unique_ptr<KeyGeneratorInterface> key_gen_;
    unsigned host_id_;
    int32_t padding_;
};

} // namespace e8

#endif // PROD_ENVIRONMENT_CONTEXT_H
