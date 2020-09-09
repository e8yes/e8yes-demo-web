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
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/pagination.pb.h"

bool RetrieveUserEntityTest() {
    e8::DemoWebTestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/123L,
                                                  env.CurrentHostId(), db_conns)
                               .value();
    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/223L,
                                                  env.CurrentHostId(), db_conns)
                               .value();

    std::optional<e8::UserEntity> retrieved = e8::RetrieveUser(/*user_id=*/123L, db_conns);
    TEST_CONDITION(retrieved.has_value());
    TEST_CONDITION(user0.id == retrieved.value().id);

    return true;
}

bool RetrieveUserEntitiesTest() {
    e8::DemoWebTestEnvironmentContext env;

    e8::UserEntity user0 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/123L,
                                                  env.CurrentHostId(), env.DemowebDatabase())
                               .value();
    e8::UserEntity user1 = e8::CreateBaselineUser(/*security_key=*/"PASS", /*user_id=*/223L,
                                                  env.CurrentHostId(), env.DemowebDatabase())
                               .value();

    std::vector<e8::UserEntity> retrieved = e8::RetrieveUsers(
        /*user_ids=*/{*user0.id.Value(), *user1.id.Value()}, env.DemowebDatabase());
    TEST_CONDITION(retrieved.size() == 2);
    TEST_CONDITION(
        std::find_if(retrieved.begin(), retrieved.end(), [&user0](e8::UserEntity const &user) {
            return *user.id.Value() == *user0.id.Value();
        }) != retrieved.end());
    TEST_CONDITION(
        std::find_if(retrieved.begin(), retrieved.end(), [&user1](e8::UserEntity const &user) {
            return *user.id.Value() == *user1.id.Value();
        }) != retrieved.end());

    return true;
}

int main() {
    e8::BeginTestSuite("user_storage");
    e8::RunTest("RetrieveUserEntityTest", RetrieveUserEntityTest);
    e8::RunTest("RetrieveUserEntitiesTest", RetrieveUserEntitiesTest);
    e8::EndTestSuite();
    return 0;
}
