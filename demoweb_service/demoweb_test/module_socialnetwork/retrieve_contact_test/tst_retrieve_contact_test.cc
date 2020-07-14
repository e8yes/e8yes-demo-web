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
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module_identity/create_user.h"
#include "demoweb_service/demoweb/module_socialnetwork/contact_invitation.h"
#include "demoweb_service/demoweb/module_socialnetwork/retrieve_contact.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"
#include "demoweb_service/demoweb/proto_cc/user_relation.pb.h"

class retrieve_contact_test : public QObject {
    Q_OBJECT

  public:
    retrieve_contact_test();
    ~retrieve_contact_test();

  private slots:
    void get_related_users_test();
};

retrieve_contact_test::retrieve_contact_test() {}

retrieve_contact_test::~retrieve_contact_test() {}

void retrieve_contact_test::get_related_users_test() {
    e8::TestEnvironmentContext env;

    std::optional<e8::UserEntity> user1 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/1,
                       env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/2,
                       env.CurrentHostId(), env.DemowebDatabase());

    e8::SendInvitation(*user1->id.Value(), *user2->id.Value(),
                       /*send_message_anyway=*/false, env.DemowebDatabase());

    std::vector<e8::UserEntity> inviters = e8::GetRelatedUsers(
        *user2->id.Value(),
        std::vector<e8::UserRelation>{e8::UserRelation::URL_INVITATION_RECEIVED},
        std::optional<e8::Pagination>(), env.DemowebDatabase());
    QVERIFY(inviters.size() == 1);
    QVERIFY(inviters[0].id.Value().value() == 1);

    // Check pagination effect.
    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);

    inviters = e8::GetRelatedUsers(
        *user2->id.Value(),
        std::vector<e8::UserRelation>{e8::UserRelation::URL_INVITATION_RECEIVED}, page1,
        env.DemowebDatabase());
    QVERIFY(inviters.size() == 1);

    e8::Pagination page2;
    page2.set_page_number(1);
    page2.set_result_per_page(2);

    inviters = e8::GetRelatedUsers(
        *user2->id.Value(),
        std::vector<e8::UserRelation>{e8::UserRelation::URL_INVITATION_RECEIVED}, page2,
        env.DemowebDatabase());
    QVERIFY(inviters.empty());
}

QTEST_APPLESS_MAIN(retrieve_contact_test)

#include "tst_retrieve_contact_test.moc"
