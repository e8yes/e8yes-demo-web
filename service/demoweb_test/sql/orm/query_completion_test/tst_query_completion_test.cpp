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
