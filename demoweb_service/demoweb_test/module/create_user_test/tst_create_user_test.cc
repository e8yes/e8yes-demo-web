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
#include <ctime>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/system_user_group.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

class create_user_test : public QObject {
    Q_OBJECT

  public:
    create_user_test();
    ~create_user_test();

  private slots:
    void create_baseline_user_test();
};

create_user_test::create_user_test() {}

create_user_test::~create_user_test() {}

void create_user_test::create_baseline_user_test() {
    e8::TestEnvironmentContext env;
    e8::ConnectionReservoirInterface *db_conns = env.DemowebDatabase();

    std::string user0_password = "PASS";
    std::optional<e8::UserEntity> user_opt =
        e8::CreateBaselineUser(user0_password,
                               /*user_id=*/std::nullopt, env.CurrentHostId(), db_conns);
    QVERIFY(user_opt.has_value());

    e8::UserEntity user = user_opt.value();

    QVERIFY(user.id.Value().has_value());
    QVERIFY(user.id_str.Value().has_value());
    QVERIFY(user.id.Value().has_value());
    QVERIFY(user.security_key_hash.Value().has_value());
    QVERIFY(!user.security_key_hash.Value().value().empty());
    QVERIFY(!user.alias.Value().has_value());
    QVERIFY(!user.avatar_path.Value().has_value());
    QVERIFY(user.emails.Value().empty());
    QVERIFY(user.created_at.Value().has_value());
    std::time_t current_timestamp;
    std::time(&current_timestamp);
    QVERIFY(current_timestamp - user.created_at.Value().value() < 2);
    QVERIFY(user.active_level.Value().has_value());
    QVERIFY(user.active_level.Value().value() == 0);
    QVERIFY(user.group_names.Value().size() == 1);
    QVERIFY(user.group_names.Value()[0] ==
            e8::kSystemUserGroupStrings[e8::SystemUserGroup::BASELINE_USER_GROUP]);
}

QTEST_APPLESS_MAIN(create_user_test)

#include "tst_create_user_test.moc"
