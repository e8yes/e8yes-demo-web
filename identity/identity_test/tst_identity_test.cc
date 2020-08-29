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

#include <QtTest>
#include <ctime>
#include <memory>
#include <optional>

#include "identity/trustable_identity.h"
#include "keygen/persistent_key_generator.h"
#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/identity.pb.h"

class identity_test : public QObject {
    Q_OBJECT

  public:
    identity_test();
    ~identity_test();

  private slots:
    void successful_sign_and_parse_test();
    void expired_signature_test();
};

identity_test::identity_test() {}

identity_test::~identity_test() {}

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*port=*/5432,
                                  /*db_name=*/"demoweb",
                                  /*user_name=*/"postgres",
                                  /*password=*/"password");
    return factory;
}

void identity_test::successful_sign_and_parse_test() {
    auto reservoir = std::make_unique<e8::BasicConnectionReservoir>(CreateConnectionFactory());
    e8::ClearAllTables(reservoir.get());

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost",
                                       /*port=*/5432,
                                       /*user_name=*/"postgres",
                                       /*password=*/"password");

    e8::Identity identity;
    identity.set_user_id(1L);
    *identity.add_group_names() = "default_group";
    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    identity.set_expiry_timestamp(cur_timestamp + 1000);

    std::optional<e8::SignedIdentity> signed_id = e8::SignIdentity(identity, &key_gen);

    QVERIFY(signed_id.has_value());

    std::optional<e8::Identity> decoded = e8::ValidateSignedIdentity(*signed_id, &key_gen);
    QVERIFY(decoded.has_value());
    QVERIFY(decoded->user_id() == 1L);
    QVERIFY(decoded->group_names_size() == 1);
    QVERIFY(decoded->group_names(0) == "default_group");
}

void identity_test::expired_signature_test() {
    auto reservoir = std::make_unique<e8::BasicConnectionReservoir>(CreateConnectionFactory());
    e8::ClearAllTables(reservoir.get());

    e8::PersistentKeyGenerator key_gen(/*host_name=*/"localhost",
                                       /*port=*/5432,
                                       /*user_name=*/"postgres",
                                       /*password=*/"password");

    e8::Identity identity;
    identity.set_user_id(1L);
    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    identity.set_expiry_timestamp(cur_timestamp - 1000);

    std::optional<e8::SignedIdentity> signed_id = e8::SignIdentity(identity, &key_gen);

    QVERIFY(signed_id.has_value());

    std::optional<e8::Identity> decoded = e8::ValidateSignedIdentity(*signed_id, &key_gen);
    QVERIFY(!decoded.has_value());
}

QTEST_APPLESS_MAIN(identity_test)

#include "tst_identity_test.moc"
