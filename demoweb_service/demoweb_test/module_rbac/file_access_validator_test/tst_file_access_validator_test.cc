#include <QtTest>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module_identity/create_user.h"
#include "demoweb_service/demoweb/module_rbac/file_access_validator.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"

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
    e8::TestEnvironmentContext env;

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
    e8::TestEnvironmentContext env;

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
