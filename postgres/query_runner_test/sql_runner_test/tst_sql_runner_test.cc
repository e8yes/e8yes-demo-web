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

#include <QtTest>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include "postgres/query_runner/connection/basic_connection_reservoir.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

class sql_runner_test : public QObject {
    Q_OBJECT

  public:
    sql_runner_test();
    ~sql_runner_test();

  private slots:
    void insert_then_query_test();
    void insert_then_delete_test();
    void insert_then_exists_test();
};

sql_runner_test::sql_runner_test() {}

sql_runner_test::~sql_runner_test() {}

class User : public e8::SqlEntityInterface {
  public:
    e8::SqlInt id = e8::SqlInt("id");
    e8::SqlStr user_name = e8::SqlStr("user_name");
    User() : SqlEntityInterface{&id, &user_name} {}
    User(User const &other)
        : SqlEntityInterface{&id, &user_name}, id(other.id), user_name(other.user_name) {}
};

class CreditCard : public e8::SqlEntityInterface {
  public:
    e8::SqlInt id = e8::SqlInt("id");
    e8::SqlInt user_id = e8::SqlInt("user_id");
    e8::SqlStr card_number = e8::SqlStr("card_number");
    CreditCard() : SqlEntityInterface{&id, &user_id, &card_number} {}
    CreditCard(CreditCard const &other)
        : SqlEntityInterface{&id, &user_id, &card_number}, id(other.id), user_id(other.user_id),
          card_number(other.card_number) {}
};

e8::ConnectionFactory CreateConnectionFactory() {
    e8::ConnectionFactory factory(e8::ConnectionFactory::PQ,
                                  /*host_name=*/"localhost",
                                  /*db_name=*/"demoweb");
    return factory;
}

void DropSchema(e8::ConnectionInterface *conn) {
    std::string dropUserTable = "DROP TABLE IF EXISTS QueryRunnerTestUser";
    std::string dropCardTable = "DROP TABLE IF EXISTS QueryRunnerTestCard";
    conn->RunUpdate(dropCardTable, e8::ConnectionInterface::QueryParams());
    conn->RunUpdate(dropUserTable, e8::ConnectionInterface::QueryParams());
}

void CreateSchema(e8::ConnectionInterface *conn) {
    std::string create_user_table = "CREATE TABLE QueryRunnerTestUser("
                                    "   id INTEGER NOT NULL, "
                                    "   user_name CHARACTER VARYING NOT NULL, "
                                    "   PRIMARY KEY (id))";
    conn->RunUpdate(create_user_table, e8::ConnectionInterface::QueryParams());
    std::string create_card_table =
        "CREATE TABLE QueryRunnerTestCard("
        "   id INTEGER NOT NULL, "
        "   user_id INTEGER NOT NULL, "
        "   card_number CHARACTER VARYING NOT NULL, "
        "   PRIMARY KEY (id), "
        "   FOREIGN KEY (user_id) REFERENCES QueryRunnerTestUser (id) ON DELETE CASCADE)";
    conn->RunUpdate(create_card_table, e8::ConnectionInterface::QueryParams());
}

void sql_runner_test::insert_then_query_test() {
    e8::ConnectionFactory factory = CreateConnectionFactory();
    e8::BasicConnectionReservoir reservoir(factory);
    e8::ConnectionInterface *conn = reservoir.Take();

    // Prepare schema.
    DropSchema(conn);
    CreateSchema(conn);

    // Run update.
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";
    uint64_t num_rows_affected = e8::Update(user,
                                            /*tableName=*/"QueryRunnerTestUser",
                                            /*replace=*/true, &reservoir);
    QVERIFY(num_rows_affected == 1);

    CreditCard card0;
    *card0.id.ValuePtr() = 10;
    *card0.card_number.ValuePtr() = "1234";
    *card0.user_id.ValuePtr() = 1;

    CreditCard card1;
    *card1.id.ValuePtr() = 11;
    *card1.card_number.ValuePtr() = "2234";
    *card1.user_id.ValuePtr() = 1;

    num_rows_affected = e8::Update(card0,
                                   /*tableName=*/"QueryRunnerTestCard",
                                   /*replace=*/true, &reservoir);
    QVERIFY(num_rows_affected == 1);

    num_rows_affected = e8::Update(card1,
                                   /*tableName=*/"QueryRunnerTestCard",
                                   /*replace=*/true, &reservoir);
    QVERIFY(num_rows_affected == 1);

    // Run query.
    std::vector<std::tuple<User, CreditCard>> results = e8::Query<User, CreditCard>(
        e8::SqlQueryBuilder().QueryPiece(
            "QueryRunnerTestUser user_info JOIN QueryRunnerTestCard "
            "cards ON cards.user_id=user_info.id ORDER BY cards.id DESC"),
        /*entity_aliases=*/{"user_info", "cards"}, &reservoir);

    QVERIFY(results.size() == 2);

    // First record.
    User retrieved_user = std::get<0>(results[0]);
    QVERIFY(retrieved_user.id.Value() == std::optional<int32_t>(1));
    QVERIFY(retrieved_user.user_name.Value() == std::optional<std::string>("user0"));

    CreditCard retrieved_card0 = std::get<1>(results[0]);
    QVERIFY(retrieved_card0.id.Value() == std::optional<int32_t>(11));
    QVERIFY(retrieved_card0.card_number.Value() == std::optional<std::string>("2234"));

    // Second record.
    User retrieved_user2 = std::get<0>(results[1]);
    QVERIFY(retrieved_user2.id.Value() == std::optional<int32_t>(1));
    QVERIFY(retrieved_user2.user_name.Value() == std::optional<std::string>("user0"));

    CreditCard retrieved_card1 = std::get<1>(results[1]);
    QVERIFY(retrieved_card1.id.Value() == std::optional<int32_t>(10));
    QVERIFY(retrieved_card1.card_number.Value() == std::optional<std::string>("1234"));

    // Clean up.
    DropSchema(conn);
    reservoir.Put(conn);
}

void sql_runner_test::insert_then_delete_test() {
    e8::ConnectionFactory factory = CreateConnectionFactory();
    e8::BasicConnectionReservoir reservoir(factory);
    e8::ConnectionInterface *conn = reservoir.Take();

    // Prepare schema.
    DropSchema(conn);
    CreateSchema(conn);

    // Prepare test data.
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";
    uint64_t num_rows_affected = e8::Update(user,
                                            /*tableName=*/"QueryRunnerTestUser",
                                            /*replace=*/true, &reservoir);
    QVERIFY(num_rows_affected == 1);

    // Delete nothing.
    num_rows_affected = e8::Delete(
        /*table_name=*/"QueryRunnerTestUser", e8::SqlQueryBuilder().QueryPiece("WHERE id!=1"),
        &reservoir);
    QVERIFY(num_rows_affected == 0);

    // Delete the user.
    num_rows_affected = e8::Delete(
        /*table_name=*/"QueryRunnerTestUser", e8::SqlQueryBuilder().QueryPiece("WHERE id=1"),
        &reservoir);
    QVERIFY(num_rows_affected == 1);

    // Clean up.
    DropSchema(conn);
    reservoir.Put(conn);
}

void sql_runner_test::insert_then_exists_test() {
    e8::ConnectionFactory factory = CreateConnectionFactory();
    e8::BasicConnectionReservoir reservoir(factory);
    e8::ConnectionInterface *conn = reservoir.Take();

    // Prepare schema.
    DropSchema(conn);
    CreateSchema(conn);

    // Prepare test data.
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";
    uint64_t num_rows_affected = e8::Update(user,
                                            /*tableName=*/"QueryRunnerTestUser",
                                            /*replace=*/true, &reservoir);
    QVERIFY(num_rows_affected == 1);

    // Run "exists" query.
    bool should_not_exist =
        !e8::Exists(e8::SqlQueryBuilder().QueryPiece("QueryRunnerTestUser WHERE id=2"), &reservoir);
    QVERIFY(should_not_exist);

    bool should_exist =
        e8::Exists(e8::SqlQueryBuilder().QueryPiece("QueryRunnerTestUser WHERE id=1"), &reservoir);
    QVERIFY(should_exist);

    // Clean up.
    DropSchema(conn);
    reservoir.Put(conn);
}

QTEST_APPLESS_MAIN(sql_runner_test)

#include "tst_sql_runner_test.moc"
