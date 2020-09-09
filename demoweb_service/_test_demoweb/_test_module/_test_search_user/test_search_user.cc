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

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/baseline_user.h"
#include "demoweb_service/demoweb/module/contact_storage.h"
#include "demoweb_service/demoweb/module/search_user.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_relation.pb.h"

bool SearchUserByIdPrefixTest() {
    e8::DemoWebTestEnvironmentContext env;
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
    std::vector<e8::UserEntity> page0 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/std::to_string(123L),
        /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(), pagination, db_conns);
    TEST_CONDITION(page0.size() == 2);
    TEST_CONDITION(page0[0].id.Value().value() == 12300L);
    TEST_CONDITION(page0[1].id.Value().value() == 12301L);

    pagination.set_page_number(1);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page1 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/std::to_string(123L),
        /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(), pagination, db_conns);
    TEST_CONDITION(page1.size() == 2);
    TEST_CONDITION(page1[0].id.Value().value() == 12302L);
    TEST_CONDITION(page1[1].id.Value().value() == 12303L);

    pagination.set_page_number(2);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page2 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/std::to_string(123L),
        /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(), pagination, db_conns);
    TEST_CONDITION(page2.empty());

    return true;
}

bool SearchUserByIdAliasTest() {
    e8::DemoWebTestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/1L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John Jr. A", std::nullopt, &user0, db_conns);

    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/2L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John Jr. A", std::nullopt, &user1, db_conns);

    e8::UserEntity user2 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/3L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John Jr. B", std::nullopt, &user2, db_conns);

    e8::UserEntity user3 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/4L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"John Jr. C", std::nullopt, &user3, db_conns);

    e8::UserEntity user4 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/5L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UpdateProfile(/*alias=*/"Stieve Jr. A", std::nullopt, &user4, db_conns);

    e8::Pagination pagination;
    pagination.set_page_number(0);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page0 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/"John Jr.", /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(),
        pagination, db_conns);
    TEST_CONDITION(page0.size() == 2);
    TEST_CONDITION(page0[0].id.Value().value() == 1L);
    TEST_CONDITION(page0[1].id.Value().value() == 2L);

    pagination.set_page_number(1);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page1 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/"John Jr.", /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(),
        pagination, db_conns);
    TEST_CONDITION(page1.size() == 2);
    TEST_CONDITION(page1[0].id.Value().value() == 3L);
    TEST_CONDITION(page1[1].id.Value().value() == 4L);

    pagination.set_page_number(2);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> page2 = e8::SearchUser(
        std::optional<e8::UserId>(),
        /*query=*/"John Jr.", /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>(),
        pagination, db_conns);
    TEST_CONDITION(page2.empty());

    return true;
}

bool SearchUserByRelationTest() {
    e8::DemoWebTestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/1L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*userId=*/2L,
                                                  env.CurrentHostId(), db_conns)
                               .value();

    bool rc = e8::CreateContact(*user0.id.Value(), *user1.id.Value(), db_conns);
    TEST_CONDITION(rc == true);

    e8::Pagination pagination;
    pagination.set_page_number(0);
    pagination.set_result_per_page(2);
    std::vector<e8::UserEntity> searched_users = e8::SearchUser(
        /*viewer_id=*/*user0.id.Value(),
        /*query=*/std::nullopt,
        /*oneof_user_relations=*/std::unordered_set<e8::UserRelation>{e8::URL_CONTACT}, pagination,
        db_conns);

    TEST_CONDITION(searched_users.size() == 1);
    TEST_CONDITION(*searched_users[0].id.Value() == *user1.id.Value());

    return true;
}

int main() {
    e8::BeginTestSuite("search_user");
    e8::RunTest("SearchUserByIdPrefixTest", SearchUserByIdPrefixTest);
    e8::RunTest("SearchUserByIdAliasTest", SearchUserByIdAliasTest);
    e8::RunTest("SearchUserByRelationTest", SearchUserByRelationTest);
    e8::EndTestSuite();
    return 0;
}
