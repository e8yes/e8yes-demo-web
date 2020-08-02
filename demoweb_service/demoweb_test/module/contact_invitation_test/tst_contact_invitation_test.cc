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
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/contact_invitation.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "proto_cc/user_relation.pb.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

class contact_invitation_test : public QObject {
    Q_OBJECT

  public:
    contact_invitation_test();
    ~contact_invitation_test();

  private slots:
    void send_invitation_store_test();
    void process_invitation_accept_test();
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

    bool result = e8::SendInvitation(*user1->id.Value(), *user2->id.Value(),
                                     /*send_message_anyway=*/false, env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder forward_query;
    forward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=1 AND cr.dst_user_id=2 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_INVITATION_SENT));
    result = e8::Exists(forward_query, env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder backward_query;
    backward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=2 AND cr.dst_user_id=1 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_INVITATION_RECEIVED));
    result = e8::Exists(backward_query, env.DemowebDatabase());
    QVERIFY(result);
}

void contact_invitation_test::process_invitation_accept_test() {
    e8::TestEnvironmentContext env;

    std::optional<e8::UserEntity> user1 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/1,
                       env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/2,
                       env.CurrentHostId(), env.DemowebDatabase());

    e8::SendInvitation(*user1->id.Value(), *user2->id.Value(),
                       /*send_message_anyway=*/false, env.DemowebDatabase());

    bool result = e8::ProcessInvitation(*user2->id.Value(), *user1->id.Value(), /*accept=*/true,
                                        env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder forward_query;
    forward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=1 AND cr.dst_user_id=2 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_CONTACT));
    result = e8::Exists(forward_query, env.DemowebDatabase());
    QVERIFY(result);

    e8::SqlQueryBuilder backward_query;
    backward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=2 AND cr.dst_user_id=1 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_CONTACT));
    result = e8::Exists(backward_query, env.DemowebDatabase());
    QVERIFY(result);
}

QTEST_APPLESS_MAIN(contact_invitation_test)

#include "tst_contact_invitation_test.moc"
