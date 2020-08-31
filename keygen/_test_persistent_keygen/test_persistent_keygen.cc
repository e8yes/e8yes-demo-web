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

#include "common/unit_test_util/unit_test_util.h"
#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*db_name=*/"demoweb");
    return factory;
}

bool KeyPersistenceTest() {
    auto reservoir = std::make_unique<e8::BasicConnectionReservoir>(CreateConnectionFactory());
    e8::ClearAllTables(reservoir.get());

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");

    e8::KeyGeneratorInterface::Key e1 =
        key_gen.KeyOf(/*encrypter=*/"E1", e8::KeyGeneratorInterface::RANDOM_512_BITS);
    TEST_CONDITION(!e1.key.empty());
    TEST_CONDITION(!e1.public_key.has_value());

    e8::KeyGeneratorInterface::Key e1_2 =
        key_gen.KeyOf(/*encrypter=*/"E1", e8::KeyGeneratorInterface::RANDOM_512_BITS);
    TEST_CONDITION(e1_2.key == e1.key);

    e8::KeyGeneratorInterface::Key e2 =
        key_gen.KeyOf(/*encrypter=*/"E2", e8::KeyGeneratorInterface::RANDOM_512_BITS);
    TEST_CONDITION(!e2.key.empty());
    TEST_CONDITION(!e2.public_key.has_value());
    TEST_CONDITION(e2.key != e1.key);

    e8::KeyGeneratorInterface::Key e1_rsa =
        key_gen.KeyOf(/*encrypter=*/"E1", e8::KeyGeneratorInterface::RSA_4096_BITS);
    TEST_CONDITION(!e1_rsa.key.empty());
    TEST_CONDITION(e1_rsa.public_key.has_value());
    TEST_CONDITION(e1_rsa.key != e1.key);

    e8::ClearAllTables(reservoir.get());

    return true;
}

int main() {
    e8::BeginTestSuite("persistent_keygen");
    e8::RunTest("KeyPersistenceTest", KeyPersistenceTest);
    e8::EndTestSuite();
    return 0;
}
