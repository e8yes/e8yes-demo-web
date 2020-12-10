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

#include <ctime>
#include <memory>
#include <optional>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "identity/trustable_identity.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/identity.pb.h"

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*db_name=*/"demoweb");
    return factory;
}

bool SuccessfulSignAndParseTest() {
    auto reservoir = std::make_unique<e8::BasicConnectionReservoir>(CreateConnectionFactory());
    e8::ClearAllTables(reservoir.get());

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");

    e8::Identity identity;
    identity.set_user_id(1L);
    *identity.add_group_names() = "default_group";
    identity.set_expiry_timestamp(e8::CurrentTimestampMicros() + 1000 * 1000 * 1000);

    std::optional<e8::SignedIdentity> signed_id = e8::SignIdentity(identity, &key_gen);

    TEST_CONDITION(signed_id.has_value());

    std::optional<e8::Identity> decoded = e8::ValidateSignedIdentity(*signed_id, &key_gen);
    TEST_CONDITION(decoded.has_value());
    TEST_CONDITION(decoded->user_id() == 1L);
    TEST_CONDITION(decoded->group_names_size() == 1);
    TEST_CONDITION(decoded->group_names(0) == "default_group");

    return true;
}

bool ExpiredSignatureTest() {
    auto reservoir = std::make_unique<e8::BasicConnectionReservoir>(CreateConnectionFactory());
    e8::ClearAllTables(reservoir.get());

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost");

    e8::Identity identity;
    identity.set_user_id(1L);
    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    identity.set_expiry_timestamp(e8::CurrentTimestampMicros() - 1000 * 1000 * 1000);

    std::optional<e8::SignedIdentity> signed_id = e8::SignIdentity(identity, &key_gen);

    TEST_CONDITION(signed_id.has_value());

    std::optional<e8::Identity> decoded = e8::ValidateSignedIdentity(*signed_id, &key_gen);
    TEST_CONDITION(!decoded.has_value());

    return true;
}

int main() {
    e8::BeginTestSuite("identity");
    e8::RunTest("SuccessfulSignAndParseTest", SuccessfulSignAndParseTest);
    e8::RunTest("ExpiredSignatureTest", ExpiredSignatureTest);
    e8::EndTestSuite();
    return 0;
}
