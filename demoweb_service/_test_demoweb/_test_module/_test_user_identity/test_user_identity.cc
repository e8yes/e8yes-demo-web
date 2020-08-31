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

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/user_identity.h"
#include "identity/trustable_identity.h"
#include "proto_cc/identity.pb.h"

bool DigestTest() {
    std::optional<e8::SecurityKeyHash> hash = e8::DigestSecurityKey("abdcd2");
    TEST_CONDITION(hash.has_value());

    return true;
}

bool SuccessfulSignAndParseTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::string security_key = "abdcd2";

    e8::UserEntity user;
    *user.id.ValuePtr() = 1L;
    *user.id_str.ValuePtr() = "1";
    user.group_names.ValuePtr()->push_back("group1");
    *user.security_key_hash.ValuePtr() = e8::DigestSecurityKey(security_key).value();
    *user.active_level.ValuePtr() = 0;

    std::optional<e8::SignedIdentity> signed_id =
        e8::SignIdentity(user, security_key, env.KeyGen());
    TEST_CONDITION(signed_id.has_value());

    std::optional<e8::Identity> identity = e8::ValidateSignedIdentity(*signed_id, env.KeyGen());
    TEST_CONDITION(identity.has_value());
    TEST_CONDITION(identity.value().user_id() == 1L);
    TEST_CONDITION(identity.value().group_names_size() == 1);
    TEST_CONDITION(identity.value().group_names(0) == "group1");

    return true;
}

bool AccessDeniedTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::string security_key = "abdcd2";

    e8::UserEntity user;
    *user.id.ValuePtr() = 1L;
    *user.id_str.ValuePtr() = "1";
    user.group_names.ValuePtr()->push_back("group1");
    *user.security_key_hash.ValuePtr() = e8::DigestSecurityKey(security_key).value();
    *user.active_level.ValuePtr() = 0;

    std::string wrong_security_key = "abdcd3";
    std::optional<e8::SignedIdentity> signed_id =
        e8::SignIdentity(user, wrong_security_key, env.KeyGen());
    TEST_CONDITION(!signed_id.has_value());

    return true;
}

int main() {
    e8::BeginTestSuite("user_identity");
    e8::RunTest("DigestTest", DigestTest);
    e8::RunTest("SuccessfulSignAndParseTest", SuccessfulSignAndParseTest);
    e8::RunTest("AccessDeniedTest", AccessDeniedTest);
    e8::EndTestSuite();
    return 0;
}
