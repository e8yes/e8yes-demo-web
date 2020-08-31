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

#include <optional>
#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "keygen/key_generator_interface.h"
#include "keygen/persistent_key_generator.h"
#include "keygen/sign_message.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*db_name=*/"demoweb");
    return factory;
}

bool SuccessfulEncodeAndDecodeMessageTest() {
    e8::ConnectionFactory fact = CreateConnectionFactory();
    e8::BasicConnectionReservoir db_conns(fact);
    e8::ClearAllTables(&db_conns);

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");
    e8::KeyGeneratorInterface::Key key_set = key_gen.KeyOf(
        "successful_encode_and_decode_message_test", e8::KeyGeneratorInterface::RSA_4096_BITS);

    std::string message({1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3});
    std::string signed_message = e8::SignMessage(message, key_set.key);
    TEST_CONDITION(!signed_message.empty());

    std::optional<std::string> decoded_message =
        e8::DecodeSignedMessage(signed_message, key_set.public_key.value());
    TEST_CONDITION(decoded_message.has_value());
    TEST_CONDITION(decoded_message.value() == message);

    return true;
}

bool EncodeAndDecodeDisruptedMessageTest() {
    e8::ConnectionFactory fact = CreateConnectionFactory();
    e8::BasicConnectionReservoir db_conns(fact);
    e8::ClearAllTables(&db_conns);

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");
    e8::KeyGeneratorInterface::Key key_set = key_gen.KeyOf(
        "encode_and_decode_disrupted_message_test", e8::KeyGeneratorInterface::RSA_4096_BITS);

    std::string message({1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3});
    std::string signed_message = e8::SignMessage(message, key_set.key);
    TEST_CONDITION(!signed_message.empty());

    signed_message[0]++;

    std::optional<std::string> decoded_message =
        e8::DecodeSignedMessage(signed_message, key_set.public_key.value());
    TEST_CONDITION(!decoded_message.has_value());

    return true;
}

int main() {
    e8::BeginTestSuite("sign_message");
    e8::RunTest("SuccessfulEncodeAndDecodeMessageTest", SuccessfulEncodeAndDecodeMessageTest);
    e8::RunTest("EncodeAndDecodeDisruptedMessageTest", EncodeAndDecodeDisruptedMessageTest);
    e8::EndTestSuite();
    return 0;
}
