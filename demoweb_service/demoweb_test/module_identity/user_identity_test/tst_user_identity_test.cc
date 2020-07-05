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

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module_identity/user_identity.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"

class user_identity_test : public QObject {
    Q_OBJECT

  public:
    user_identity_test();
    ~user_identity_test();

  private slots:
    void digest_test();
    void successful_sign_and_parse_test();
    void access_denied_test();
};

user_identity_test::user_identity_test() {}

user_identity_test::~user_identity_test() {}

void user_identity_test::digest_test() {
    std::optional<e8::SecurityKeyHash> hash = e8::DigestSecurityKey("abdcd2");
    QVERIFY(hash.has_value());
}

void user_identity_test::successful_sign_and_parse_test() {
    e8::TestEnvironmentContext env;

    std::string security_key = "abdcd2";

    e8::UserEntity user;
    *user.id.ValuePtr() = 1L;
    *user.id_str.ValuePtr() = "1";
    user.group_names.ValuePtr()->push_back("group1");
    *user.security_key_hash.ValuePtr() = e8::DigestSecurityKey(security_key).value();
    *user.active_level.ValuePtr() = 0;

    std::optional<e8::SignedIdentity> signed_id =
        e8::SignIdentity(user, security_key, env.KeyGen());
    QVERIFY(signed_id.has_value());

    std::optional<e8::Identity> identity =
        e8::ValidateSignedIdentity(signed_id.value(), env.KeyGen());
    QVERIFY(identity.has_value());
    QVERIFY(identity.value().user_id() == 1L);
    QVERIFY(identity.value().group_names_size() == 1);
    QVERIFY(identity.value().group_names(0) == "group1");
}

void user_identity_test::access_denied_test() {
    e8::TestEnvironmentContext env;

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
    QVERIFY(!signed_id.has_value());
}

QTEST_APPLESS_MAIN(user_identity_test)

#include "tst_user_identity_test.moc"
