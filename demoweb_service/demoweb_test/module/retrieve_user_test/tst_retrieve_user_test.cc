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
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/retrieve_user.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

class retrieve_user_test : public QObject {
    Q_OBJECT

  public:
    retrieve_user_test() = default;
    ~retrieve_user_test() = default;

  private slots:
    void retrieve_user_entity_test();
    void search_user_by_id_prefix_test();
    void search_user_by_id_alias_test();
};

void retrieve_user_test::retrieve_user_entity_test() {
    e8::TestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/123L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/223L,
                                                  env.CurrentHostId(), db_conns)
                               .value();

    std::optional<e8::UserEntity> retrieved = e8::RetrieveUser(/*user_id=*/123L, db_conns);
    QVERIFY(retrieved.has_value());
    QVERIFY(user0.id == retrieved.value().id);
}

void retrieve_user_test::search_user_by_id_prefix_test() {
    e8::TestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/12300L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/12301L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user2 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/12302L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user3 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/12303L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user4 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/22300L,
                                                  env.CurrentHostId(), db_conns)
                               .value();

    e8::Pagination pagination;
    pagination.set_page_number(0);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page0 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/123L,
                       /*alias_prefix=*/std::nullopt, pagination, db_conns);
    QVERIFY(page0.size() == 2);
    QVERIFY(page0[0].id.Value().value() == 12300L);
    QVERIFY(page0[1].id.Value().value() == 12301L);

    pagination.set_page_number(1);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page1 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/123L,
                       /*alias_prefix=*/std::nullopt, pagination, db_conns);
    QVERIFY(page1.size() == 2);
    QVERIFY(page1[0].id.Value().value() == 12302L);
    QVERIFY(page1[1].id.Value().value() == 12303L);

    pagination.set_page_number(2);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page2 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/123L,
                       /*alias_prefix=*/std::nullopt, pagination, db_conns);
    QVERIFY(page2.empty());
}

void retrieve_user_test::search_user_by_id_alias_test() {
    e8::TestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/1L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John A", &user0, db_conns);

    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/2L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John A", &user1, db_conns);

    e8::UserEntity user2 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/3L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John B", &user2, db_conns);

    e8::UserEntity user3 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/4L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John C", &user3, db_conns);

    e8::UserEntity user4 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/5L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"Stieve A", &user4, db_conns);

    e8::Pagination pagination;
    pagination.set_page_number(0);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page0 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/std::nullopt,
                       /*alias_prefix=*/"John", pagination, db_conns);
    QVERIFY(page0.size() == 2);
    QVERIFY(page0[0].id.Value().value() == 1L);
    QVERIFY(page0[1].id.Value().value() == 2L);

    pagination.set_page_number(1);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page1 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/std::nullopt,
                       /*alias_prefix=*/"John", pagination, db_conns);
    QVERIFY(page1.size() == 2);
    QVERIFY(page1[0].id.Value().value() == 3L);
    QVERIFY(page1[1].id.Value().value() == 4L);

    pagination.set_page_number(2);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page2 =
        e8::SearchUser(std::optional<e8::UserId>(),
                       /*user_id_prefix=*/std::nullopt,
                       /*alias_prefix=*/"John", pagination, db_conns);
    QVERIFY(page2.empty());
}

QTEST_APPLESS_MAIN(retrieve_user_test)

#include "tst_retrieve_user_test.moc"
