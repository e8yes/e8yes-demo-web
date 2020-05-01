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
#include <optional>

#include "sql/connection/connection_interface.h"
#include "sql/reflection/sql_primitives.h"
#include "sql/sql_query_builder.h"

class sql_query_builder_test : public QObject {
    Q_OBJECT

  public:
    sql_query_builder_test();
    ~sql_query_builder_test();

  private slots:
    void build_parameterized_query_test();
};

sql_query_builder_test::sql_query_builder_test() {}

sql_query_builder_test::~sql_query_builder_test() {}

void sql_query_builder_test::build_parameterized_query_test() {
    e8::SqlQueryBuilder::Placeholder<e8::SqlInt> user_id;
    e8::SqlQueryBuilder::Placeholder<e8::SqlTimestamp> begin_timestamp;
    e8::SqlQueryBuilder::Placeholder<e8::SqlTimestamp> end_timestamp;

    e8::SqlQueryBuilder builder;
    builder.query_piece("Security s JOIN Image im ON s.id=im.securityId AND ")
        .query_piece("s.userId=")
        .placeholder(&user_id)
        .query_piece(" WHERE im.date>=")
        .placeholder(&begin_timestamp)
        .query_piece(" AND im.date<")
        .placeholder(&end_timestamp)
        .query_piece(" AND im.blockId!=")
        .placeholder(&user_id);

    QVERIFY(builder.psql_query() ==
            "Security s JOIN Image im ON s.id=im.securityId AND s.userId=$1 "
            "WHERE im.date>=$2 AND im.date<$3 AND im.blockId!=$4");

    e8::SqlInt user_id_value(1);
    e8::SqlTimestamp begin_timestamp_value(100);
    e8::SqlTimestamp end_timestamp_value(101);
    builder.set_value_to_placeholder(user_id, &user_id_value);
    builder.set_value_to_placeholder(begin_timestamp, &begin_timestamp_value);
    builder.set_value_to_placeholder(end_timestamp, &end_timestamp_value);

    e8::ConnectionInterface::QueryParams const &params = builder.query_params();
    QVERIFY(params.parameters().size() == 4);
    QVERIFY(*params.get_param(1) == user_id_value);
    QVERIFY(*params.get_param(2) == begin_timestamp_value);
    QVERIFY(*params.get_param(3) == end_timestamp_value);
    QVERIFY(*params.get_param(4) == user_id_value);

    e8::SqlInt updated_user_id_value(2);
    builder.set_value_to_placeholder(user_id, &updated_user_id_value);

    e8::ConnectionInterface::QueryParams const &updated_params = builder.query_params();
    QVERIFY(updated_params.parameters().size() == 4);
    QVERIFY(*updated_params.get_param(1) == user_id_value);
    QVERIFY(*updated_params.get_param(2) == begin_timestamp_value);
    QVERIFY(*updated_params.get_param(3) == end_timestamp_value);
    QVERIFY(*updated_params.get_param(4) == user_id_value);
}

QTEST_APPLESS_MAIN(sql_query_builder_test)

#include "tst_sql_query_builder_test.moc"
