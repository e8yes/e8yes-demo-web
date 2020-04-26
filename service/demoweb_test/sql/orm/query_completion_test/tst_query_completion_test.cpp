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
#include <string>

#include "sql/orm/query_completion.h"
#include "sql/reflection/sql_entity_interface.h"
#include "sql/reflection/sql_primitives.h"

class query_completion_test : public QObject {
    Q_OBJECT

  public:
    query_completion_test();
    ~query_completion_test();

  private slots:
    void test_select_query_completion();
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

void query_completion_test::test_select_query_completion() {
    User user;

    std::string query = "AUser auser JOIN CreditCard card ON card.user_id = auser.id WHERE "
                        "auser.join_date > '2020-1-1'";
    std::string actual = e8::CompleteSelectQuery<User, CreditCard>(query, {"auser", "card"});
    QVERIFY(
        actual ==
        "SELECT auser.id,auser.user_name,card.id,card.user_id,card.card_number FROM AUser auser "
        "JOIN CreditCard card ON card.user_id = auser.id WHERE auser.join_date > "
        "'2020-1-1'");
}

QTEST_APPLESS_MAIN(query_completion_test)

#include "tst_query_completion_test.moc"
