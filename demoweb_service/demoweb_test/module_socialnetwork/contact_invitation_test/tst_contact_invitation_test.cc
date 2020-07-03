#include <QtTest>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module_identity/create_user.h"
#include "demoweb_service/demoweb/module_socialnetwork/contact_invitation.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

class contact_invitation_test : public QObject {
    Q_OBJECT

  public:
    contact_invitation_test();
    ~contact_invitation_test();

  private slots:
    void send_invitation_store_test();
};

contact_invitation_test::contact_invitation_test() {}

contact_invitation_test::~contact_invitation_test() {}

void contact_invitation_test::send_invitation_store_test() {
    e8::TestEnvironmentContext env;

    std::optional<e8::UserEntity> user1 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/1,
                       env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/2,
                       env.CurrentHostId(), env.DemowebDatabase());

    bool result = e8::SendInvitation(user1->id.value().value(), user2->id.value().value(),
                                     /*send_message_anyway=*/false, env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder forward_query;
    forward_query.query_piece(e8::TableNames::ContactRelation())
        .query_piece(" cr ")
        .query_piece("WHERE cr.src_user_id=1 AND cr.dst_user_id=2 AND cr.relation=1");
    result = e8::Exists(forward_query, env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder backward_query;
    backward_query.query_piece(e8::TableNames::ContactRelation())
        .query_piece(" cr ")
        .query_piece("WHERE cr.src_user_id=2 AND cr.dst_user_id=1 AND cr.relation=2");
    result = e8::Exists(backward_query, env.DemowebDatabase());
    QVERIFY(result);
}

QTEST_APPLESS_MAIN(contact_invitation_test)

#include "tst_contact_invitation_test.moc"
