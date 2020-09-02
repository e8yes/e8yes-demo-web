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

#include <string>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/module/user_profile.h"
#include "proto_cc/file.pb.h"

// TODO: enrich this test suite.

bool AllocateNewAvatarLocationTest() {
    std::string path = e8::profile_internal::AllocateNewAvatarLocation(
        "123", e8::FileFormat::FFMT_IMAGE_PNG, std::nullopt);
    TEST_CONDITION(path == "/user/123/avatar/1.png");

    return true;
}

bool AllocateNewAvatarLocationWithOldPathTest() {
    std::string path = e8::profile_internal::AllocateNewAvatarLocation(
        "123", e8::FileFormat::FFMT_IMAGE_PNG, "/user/123/avatar/1.png");
    TEST_CONDITION(path == "/user/123/avatar/2.png");

    return true;
}

int main() {
    e8::BeginTestSuite("user_profile");
    e8::RunTest("AllocateNewAvatarLocationTest", AllocateNewAvatarLocationTest);
    e8::RunTest("AllocateNewAvatarLocationWithOldPathTest",
                AllocateNewAvatarLocationWithOldPathTest);
    e8::EndTestSuite();
    return 0;
}
