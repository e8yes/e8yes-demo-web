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

#include <optional>
#include <string>

#include "common/time_util/time_util.h"
#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/baseline_user.h"
#include "demoweb_service/demoweb/module/system_user_group.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

bool CreateBaselineUserTest() {
    e8::DemoWebTestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    std::string user0_password = "PASS";
    std::optional<e8::UserEntity> user_opt =
        e8::CreateBaselineUser(user0_password,
                               /*user_id=*/std::nullopt, env.CurrentHostId(), db_conns);
    TEST_CONDITION(user_opt.has_value());

    e8::UserEntity user = user_opt.value();

    TEST_CONDITION(user.id.Value().has_value());
    TEST_CONDITION(user.id_str.Value().has_value());
    TEST_CONDITION(user.id.Value().has_value());
    TEST_CONDITION(user.security_key_hash.Value().has_value());
    TEST_CONDITION(!user.security_key_hash.Value().value().empty());
    TEST_CONDITION(!user.alias.Value().has_value());
    TEST_CONDITION(!user.avatar_path.Value().has_value());
    TEST_CONDITION(user.emails.Value().empty());
    TEST_CONDITION(user.created_at.Value().has_value());
    TEST_CONDITION(e8::CurrentTimestampMicros() - user.created_at.Value().value() < 1000 * 1000);
    TEST_CONDITION(user.active_level.Value().has_value());
    TEST_CONDITION(user.active_level.Value().value() == 0);
    TEST_CONDITION(user.group_names.Value().size() == 1);
    TEST_CONDITION(user.group_names.Value()[0] ==
                   e8::kSystemUserGroupStrings[e8::SystemUserGroup::BASELINE_USER_GROUP]);

    return true;
}

int main() {
    e8::BeginTestSuite("create_user");
    e8::RunTest("CreateBaselineUserTest", CreateBaselineUserTest);
    e8::EndTestSuite();
    return 0;
}
