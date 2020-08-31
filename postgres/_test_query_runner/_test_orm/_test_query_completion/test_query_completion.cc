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

#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "postgres/query_runner/orm/query_completion.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

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

bool SelectQueryCompletionTest() {
    std::string query = "AUser auser JOIN CreditCard card ON card.user_id = auser.id WHERE "
                        "auser.join_date > '2020-1-1'";
    std::string actual = e8::CompleteSelectQuery<User, CreditCard>(query, {"auser", "card"});
    TEST_CONDITION(
        actual ==
        "SELECT auser.id,auser.user_name,card.id,card.user_id,card.card_number FROM AUser auser "
        "JOIN CreditCard card ON card.user_id = auser.id WHERE auser.join_date > "
        "'2020-1-1'");

    return true;
}

bool GenerateInsertQueryTest() {
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";

    e8::InsertQueryAndParams query_and_params =
        e8::GenerateInsertQuery(/*table_name=*/"AUser", user, /*with_upsert=*/false);
    TEST_CONDITION(query_and_params.query ==
                   "INSERT INTO AUser(id,user_name)VALUES($1,$2)ON CONFLICT DO NOTHING");

    TEST_CONDITION(*query_and_params.query_params.GetParam(0) == user.id);
    TEST_CONDITION(*query_and_params.query_params.GetParam(1) == user.user_name);

    return true;
}

bool GenerateUpsertQueryTest() {
    User user;
    *user.id.ValuePtr() = 1;
    *user.user_name.ValuePtr() = "user0";

    e8::InsertQueryAndParams query_and_params =
        e8::GenerateInsertQuery(/*table_name=*/"AUser", user, /*with_upsert=*/true);
    TEST_CONDITION(query_and_params.query ==
                   "INSERT INTO AUser(id,user_name)VALUES($1,$2)ON CONFLICT ON "
                   "CONSTRAINT AUser_pkey DO UPDATE SET id=$3,user_name=$4");

    TEST_CONDITION(*query_and_params.query_params.GetParam(0) == user.id);
    TEST_CONDITION(*query_and_params.query_params.GetParam(1) == user.user_name);
    TEST_CONDITION(*query_and_params.query_params.GetParam(2) == user.id);
    TEST_CONDITION(*query_and_params.query_params.GetParam(3) == user.user_name);

    return true;
}

int main() {
    e8::BeginTestSuite("query_completion");
    e8::RunTest("SelectQueryCompletionTest", SelectQueryCompletionTest);
    e8::RunTest("GenerateInsertQueryTest", GenerateInsertQueryTest);
    e8::RunTest("GenerateUpsertQueryTest", GenerateUpsertQueryTest);
    e8::EndTestSuite();
    return 0;
}
