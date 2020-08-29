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
#include <string>

#include "demoweb_service/demoweb/module/user_profile.h"
#include "proto_cc/file.pb.h"

class user_profile_test : public QObject {
    Q_OBJECT

  public:
    user_profile_test();
    ~user_profile_test();

  private slots:
    void allocate_new_avatar_location_test();
    void allocate_new_avatar_location_with_old_path_test();
};

user_profile_test::user_profile_test() {}

user_profile_test::~user_profile_test() {}

void user_profile_test::allocate_new_avatar_location_test() {
    std::string path = e8::profile_internal::AllocateNewAvatarLocation(
        "123", e8::FileFormat::FFMT_IMAGE_PNG, std::nullopt);
    QVERIFY(path == "/user/123/avatar/1.png");
}

void user_profile_test::allocate_new_avatar_location_with_old_path_test() {
    std::string path = e8::profile_internal::AllocateNewAvatarLocation(
        "123", e8::FileFormat::FFMT_IMAGE_PNG, "/user/123/avatar/1.png");
    QVERIFY(path == "/user/123/avatar/2.png");
}

QTEST_APPLESS_MAIN(user_profile_test)

#include "tst_user_profile_test.moc"
