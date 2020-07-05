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
#include <optional>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"

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
    builder.QueryPiece("Security s JOIN Image im ON s.id=im.securityId AND ")
        .QueryPiece("s.userId=")
        .Holder(&user_id)
        .QueryPiece(" WHERE im.date>=")
        .Holder(&begin_timestamp)
        .QueryPiece(" AND im.date<")
        .Holder(&end_timestamp)
        .QueryPiece(" AND im.blockId!=")
        .Holder(&user_id);

    QVERIFY(builder.PsqlQuery() == "Security s JOIN Image im ON s.id=im.securityId AND s.userId=$1 "
                                   "WHERE im.date>=$2 AND im.date<$3 AND im.blockId!=$4");

    auto user_id_value = std::make_shared<e8::SqlInt>(1);
    auto begin_timestamp_value = std::make_shared<e8::SqlTimestamp>(100);
    auto end_timestamp_value = std::make_shared<e8::SqlTimestamp>(101);
    builder.SetValueToPlaceholder(user_id, user_id_value);
    builder.SetValueToPlaceholder(begin_timestamp, begin_timestamp_value);
    builder.SetValueToPlaceholder(end_timestamp, end_timestamp_value);

    e8::ConnectionInterface::QueryParams const &params = builder.QueryParams();
    QVERIFY(params.Parameters().size() == 4);
    QVERIFY(*params.GetParam(1) == *user_id_value);
    QVERIFY(*params.GetParam(2) == *begin_timestamp_value);
    QVERIFY(*params.GetParam(3) == *end_timestamp_value);
    QVERIFY(*params.GetParam(4) == *user_id_value);

    auto updated_user_id_value = std::make_shared<e8::SqlInt>(2);
    builder.SetValueToPlaceholder(user_id, updated_user_id_value);

    e8::ConnectionInterface::QueryParams const &updated_params = builder.QueryParams();
    QVERIFY(updated_params.Parameters().size() == 4);
    QVERIFY(*updated_params.GetParam(1) == *user_id_value);
    QVERIFY(*updated_params.GetParam(2) == *begin_timestamp_value);
    QVERIFY(*updated_params.GetParam(3) == *end_timestamp_value);
    QVERIFY(*updated_params.GetParam(4) == *user_id_value);
}

QTEST_APPLESS_MAIN(sql_query_builder_test)

#include "tst_sql_query_builder_test.moc"
