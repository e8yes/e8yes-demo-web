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
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/pq_connection.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

class pq_connection_test : public QObject {
    Q_OBJECT

  public:
    pq_connection_test();
    ~pq_connection_test();

  private slots:
    void connection_state_test();
    void update_and_query_test();
};

pq_connection_test::pq_connection_test() {}

pq_connection_test::~pq_connection_test() {}

void pq_connection_test::connection_state_test() {
    e8::PqConnection conn(
        /*host_name=*/"localhost",
        /*port=*/5432,
        /*db_name=*/"demoweb",
        /*user_name=*/"postgres",
        /*password=*/"password");
    QVERIFY(!conn.is_closed());
}

void pq_connection_test::update_and_query_test() {
    e8::PqConnection conn(
        /*host_name=*/"localhost",
        /*port=*/5432,
        /*db_name=*/"demoweb",
        /*user_name=*/"postgres",
        /*password=*/"password");

    // Prepares table.
    std::string drop_table_stmt = "DROP TABLE IF EXISTS PqConnectionTest";
    conn.run_update(drop_table_stmt, e8::ConnectionInterface::QueryParams());

    std::string createTableStmt = "CREATE TABLE PqConnectionTest("
                                  " id BIGINT NOT NULL,"
                                  " test_name CHARACTER VARYING NOT NULL,"
                                  " PRIMARY KEY (id))";
    conn.run_update(createTableStmt, e8::ConnectionInterface::QueryParams());

    // Insert test records.
    std::string insert_stmt = "INSERT INTO PqConnectionTest(id, test_name) VALUES ($1, $2)";
    e8::ConnectionInterface::QueryParams insertion_params;
    e8::SqlLong id_param(1L);
    e8::SqlStr str_param("test_string", "");
    insertion_params.set_param(1, &id_param);
    insertion_params.set_param(2, &str_param);
    uint64_t num_rows_affected = conn.run_update(insert_stmt, insertion_params);

    QVERIFY(num_rows_affected == 1);

    // Query records.
    std::string query_stmt = "SELECT * FROM PqConnectionTest WHERE id=$1";
    e8::ConnectionInterface::QueryParams query_params;
    e8::SqlLong id_query_param(1L);
    query_params.set_param(1, &id_query_param);

    std::unique_ptr<e8::ResultSetInterface> rs = conn.run_query(query_stmt, query_params);
    QVERIFY(rs->has_next());

    e8::SqlLong id_field("id");
    e8::SqlStr test_name_field("test_name");
    rs->set_field(0, &id_field);
    rs->set_field(1, &test_name_field);

    QVERIFY(id_field.value() == std::optional<int64_t>(1L));
    QVERIFY(test_name_field.value() == std::optional<std::string>("test_string"));

    rs->next();
    QVERIFY(!rs->has_next());

    // Clean up.
    conn.run_update(drop_table_stmt, e8::ConnectionInterface::QueryParams());
}

QTEST_APPLESS_MAIN(pq_connection_test)

#include "tst_pq_connection_test.moc"
