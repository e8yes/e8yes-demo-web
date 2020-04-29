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
#include <ctime>
#include <memory>
#include <string>

#include "sql/connection/connection_interface.h"
#include "sql/connection/pq_connection.h"
#include "sql/reflection/sql_primitives.h"
#include "sql/resultset/pq_result_set.h"
#include "sql/resultset/result_set_interface.h"

class pq_result_set_test : public QObject {
    Q_OBJECT

  public:
    pq_result_set_test();
    ~pq_result_set_test();

  private slots:
    void insert_and_retrieve_test();
};

pq_result_set_test::pq_result_set_test() {}

pq_result_set_test::~pq_result_set_test() {}

class Record {
  public:
    e8::SqlInt int_field = e8::SqlInt("int_field");
    e8::SqlLong long_field = e8::SqlLong("long_filed");
    e8::SqlBool bool_field = e8::SqlBool("bool_field");
    e8::SqlFloat float_field = e8::SqlFloat("float_field");
    e8::SqlDouble double_field = e8::SqlDouble("double_field");
    e8::SqlTimestamp timestamp_field = e8::SqlTimestamp("timestamp_field");
    e8::SqlStr string_field = e8::SqlStr("string_field");
    e8::SqlIntArr int_array_field = e8::SqlIntArr("int_array_field");
    e8::SqlLongArr long_array_field = e8::SqlLongArr("long_array_field");
    e8::SqlBoolArr bool_array_field = e8::SqlBoolArr("bool_array_field");
    e8::SqlFloatArr float_array_field = e8::SqlFloatArr("float_array_field");
    e8::SqlDoubleArr double_array_field = e8::SqlDoubleArr("double_array_field");
    e8::SqlTimestampArr timestamp_array_field = e8::SqlTimestampArr("date_array_field");
    e8::SqlStrArr string_array_field = e8::SqlStrArr("string_array_field");
};

void pq_result_set_test::insert_and_retrieve_test() {
    e8::PqConnection conn(
        /*host_name=*/"localhost",
        /*port=*/5432,
        /*db_name=*/"demowebtest",
        /*user_name=*/"postgres",
        /*password=*/"password");
    std::string drop_table_stmt = "DROP TABLE IF EXISTS PqResultSetTestTable";
    std::string create_table_stmt = "CREATE TABLE IF NOT EXISTS PqResultSetTestTable ("
                                    "   int_field INT, "
                                    "   long_field BIGINT, "
                                    "   bool_field BOOLEAN, "
                                    "   float_field REAL, "
                                    "   double_field DOUBLE PRECISION, "
                                    "   timestamp_field TIMESTAMP WITHOUT TIME ZONE, "
                                    "   string_field CHARACTER VARYING, "
                                    "   int_array_field INT[], "
                                    "   long_array_field BIGINT[], "
                                    "   bool_array_field BOOLEAN[], "
                                    "   float_array_field REAL[], "
                                    "   double_array_field DOUBLE PRECISION[], "
                                    "   timestamp_array_field TIMESTAMP WITHOUT TIME ZONE[], "
                                    "   string_array_field CHARACTER VARYING[] "
                                    ")";
    conn.run_update(drop_table_stmt, e8::ConnectionInterface::QueryParams());
    conn.run_update(create_table_stmt, e8::ConnectionInterface::QueryParams());

    std::string insert_record_stmt =
        "INSERT INTO PqResultSetTestTable ("
        "   int_field, "
        "   long_field, "
        "   bool_field, "
        "   float_field, "
        "   double_field, "
        "   timestamp_field,"
        "   string_field, "
        "   int_array_field, "
        "   long_array_field, "
        "   bool_array_field, "
        "   float_array_field, "
        "   double_array_field, "
        "   timestamp_array_field, "
        "   string_array_field) "
        "VALUES ("
        "   10, "
        "   100, "
        "   TRUE, "
        "   0.5, "
        "   0.25, "
        "   to_timestamp(CAST(111 AS BIGINT)) AT TIME ZONE 'UTC', "
        "   'string_value', "
        "   ARRAY[10, 11], "
        "   ARRAY[100, 101], "
        "   ARRAY[TRUE, FALSE], "
        "   ARRAY[0.5, 1.5], "
        "   ARRAY[0.25, 1.25], "
        "   ARRAY[to_timestamp(CAST(111 AS BIGINT)) AT TIME ZONE 'UTC', "
        "         to_timestamp(CAST(112 AS BIGINT)) AT TIME ZONE 'UTC'], "
        "   ARRAY['string_value0', 'string_value1'] "
        ")";
    uint64_t num_rows_inserted =
        conn.run_update(insert_record_stmt, e8::ConnectionInterface::QueryParams());
    QVERIFY(num_rows_inserted == 1);

    std::string query_stmt = "SELECT * FROM PqResultSetTestTable";
    std::unique_ptr<e8::ResultSetInterface> rs =
        conn.run_query(query_stmt, e8::ConnectionInterface::QueryParams());
    QVERIFY(rs->has_next());

    Record record;
    rs->set_field(0, &record.int_field);
    rs->set_field(1, &record.long_field);
    rs->set_field(2, &record.bool_field);
    rs->set_field(3, &record.float_field);
    rs->set_field(4, &record.double_field);
    rs->set_field(5, &record.timestamp_field);
    rs->set_field(6, &record.string_field);
    rs->set_field(7, &record.int_array_field);
    rs->set_field(8, &record.long_array_field);
    rs->set_field(9, &record.bool_array_field);
    rs->set_field(10, &record.float_array_field);
    rs->set_field(11, &record.double_array_field);
    rs->set_field(12, &record.timestamp_array_field);
    rs->set_field(13, &record.string_array_field);

    QVERIFY(record.int_field.value() == 10);
    QVERIFY(record.long_field.value() == 100);
    QVERIFY(record.bool_field.value() == true);
    QVERIFY(record.float_field.value() == 0.5f);
    QVERIFY(record.double_field.value() == 0.25);
    QVERIFY(record.timestamp_field.value() == 111);
    QVERIFY(record.string_field.value() == "string_value");
    QVERIFY(record.int_array_field.value() == std::vector<int32_t>({10, 11}));
    QVERIFY(record.long_array_field.value() == std::vector<int64_t>({100, 101}));
    QVERIFY(record.bool_array_field.value() == std::vector<bool>({true, false}));
    QVERIFY(record.float_array_field.value() == std::vector<float>({0.5f, 1.5f}));
    QVERIFY(record.double_array_field.value() == std::vector<double>({0.25, 1.25}));
    QVERIFY(record.timestamp_array_field.value() == std::vector<std::time_t>({111, 112}));
    QVERIFY(record.string_array_field.value() ==
            std::vector<std::string>({"string_value0", "string_value1"}));

    rs->next();
    QVERIFY(!rs->has_next());

    conn.run_update(drop_table_stmt, e8::ConnectionInterface::QueryParams());
}

QTEST_APPLESS_MAIN(pq_result_set_test)

#include "tst_pq_result_set_test.moc"