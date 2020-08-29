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
#include <string>

#include "postgres/query_runner/orm/query_completion.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

class query_completion_test : public QObject {
    Q_OBJECT

  public:
    query_completion_test();
    ~query_completion_test();

  private slots:
    void select_query_completion_test();
    void generate_insert_query_test();
    void generate_upsert_query_test();
};

query_completion_test::query_completion_test() {}

query_completion_test::~query_completion_test() {}

class User : public e8::SqlEntityInterface {
  public:
    e8::SqlInt id = e8::SqlInt("id");
    e8::SqlStr user_name = e8::SqlStr("user_name");
    User() : SqlEntityInterface{&id, &user_name} {}
};

class CreditCard : public e8::SqlEntityInterface {
  public:
    e8::SqlInt id = e8::SqlInt("id");
    e8::SqlInt user_id = e8::SqlInt("user_id");
    e8::SqlStr card_number = e8::SqlStr("card_number");
    CreditCard() : SqlEntityInterface{&id, &user_id, &card_number} {}
};

void query_completion_test::select_query_completion_test() {
    std::string query = "AUser auser JOIN CreditCard card ON card.user_id = auser.id WHERE "
                        "auser.join_date > '2020-1-1'";
    std::string actual = e8::CompleteSelectQuery<User, CreditCard>(query, {"auser", "card"});
    QVERIFY(
        actual ==
        "SELECT auser.id,auser.user_name,card.id,card.user_id,card.card_number FROM AUser auser "
        "JOIN CreditCard card ON card.user_id = auser.id WHERE auser.join_date > "
        "'2020-1-1'");
}

void query_completion_test::generate_insert_query_test() {
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";

    e8::InsertQueryAndParams query_and_params =
        e8::GenerateInsertQuery(/*table_name=*/"AUser", user, /*with_upsert=*/false);
    QVERIFY(query_and_params.query ==
            "INSERT INTO AUser(id,user_name)VALUES($1,$2)ON CONFLICT DO NOTHING");

    QVERIFY(*query_and_params.query_params.GetParam(0) == user.id);
    QVERIFY(*query_and_params.query_params.GetParam(1) == user.user_name);
}

void query_completion_test::generate_upsert_query_test() {
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";

    e8::InsertQueryAndParams query_and_params =
        e8::GenerateInsertQuery(/*table_name=*/"AUser", user, /*with_upsert=*/true);
    QVERIFY(query_and_params.query == "INSERT INTO AUser(id,user_name)VALUES($1,$2)ON CONFLICT ON "
                                      "CONSTRAINT AUser_pkey DO UPDATE SET id=$3,user_name=$4");

    QVERIFY(*query_and_params.query_params.GetParam(0) == user.id);
    QVERIFY(*query_and_params.query_params.GetParam(1) == user.user_name);
    QVERIFY(*query_and_params.query_params.GetParam(2) == user.id);
    QVERIFY(*query_and_params.query_params.GetParam(3) == user.user_name);
}

QTEST_APPLESS_MAIN(query_completion_test)

#include "tst_query_completion_test.moc"
