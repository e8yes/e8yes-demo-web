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

#include <QtTest>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/file_access_validator.h"
#include "proto_cc/identity.pb.h"

class file_access_validator_test : public QObject {
    Q_OBJECT

  public:
    file_access_validator_test();
    ~file_access_validator_test();

  private slots:
    void access_token_validation_test();
    void direct_access_validation_test();
};

file_access_validator_test::file_access_validator_test() {}

file_access_validator_test::~file_access_validator_test() {}

void file_access_validator_test::access_token_validation_test() {
    e8::DemoWebTestEnvironmentContext env;

    e8::UserId user_id = 123L;
    std::string file_path = "/user/123/avatar/face.png";
    e8::FileAccessToken token =
        e8::SignFileAccessToken(user_id, file_path, e8::FileAccessMode::FAM_READ, env.KeyGen());
    QVERIFY(!token.empty());

    // Expact successful verification.
    std::optional<std::string> decoded_file_path =
        e8::ValidateFileAccessToken(user_id, e8::FileAccessMode::FAM_READ, token, env.KeyGen());
    QVERIFY(decoded_file_path.has_value());
    QVERIFY(decoded_file_path.value() == file_path);

    // Impersonation.
    decoded_file_path =
        e8::ValidateFileAccessToken(user_id + 1, e8::FileAccessMode::FAM_READ, token, env.KeyGen());
    QVERIFY(!decoded_file_path.has_value());

    // Bad access mode.
    decoded_file_path = e8::ValidateFileAccessToken(user_id, e8::FileAccessMode::FAM_READWRITE,
                                                    token, env.KeyGen());
    QVERIFY(!decoded_file_path.has_value());
}

void file_access_validator_test::direct_access_validation_test() {
    e8::DemoWebTestEnvironmentContext env;

    e8::UserId user_id = 123L;
    std::optional<e8::UserEntity> user = e8::CreateUser("abc", std::vector<std::string>(), user_id,
                                                        env.CurrentHostId(), env.DemowebDatabase());
    e8::Identity identity;
    identity.set_user_id(user_id);

    std::string file_path = "/user/123/avatar/face.png";

    // Should fail because the user is not party of any file groups.
    bool result = e8::ValidateDirectFileAccess(identity, file_path, e8::FileAccessMode::FAM_READ,
                                               env.DemowebDatabase());
    QVERIFY(!result);

    // TODO: complete the remaining test once the user group module is implemented.
}

QTEST_APPLESS_MAIN(file_access_validator_test)

#include "tst_file_access_validator_test.moc"
