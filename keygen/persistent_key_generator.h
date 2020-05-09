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

#ifndef PERSISTENT_KEY_GENERATOR_H
#define PERSISTENT_KEY_GENERATOR_H

#include <memory>
#include <string>

#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief The PersistentKeyGenerator class Persistence relies on the postgres database.
 */
class PersistentKeyGenerator : public KeyGeneratorInterface {
  public:
    PersistentKeyGenerator(std::unique_ptr<ConnectionReservoirInterface> reservoir);
    ~PersistentKeyGenerator() override;

    Key KeyOf(std::string const &encrypter, KeyType key_type) override;

  private:
    class PersistentKeyGeneratorImpl;
    std::unique_ptr<PersistentKeyGeneratorImpl> impl;
};

} // namespace e8

#endif // PERSISTENT_KEY_GENERATOR_H
