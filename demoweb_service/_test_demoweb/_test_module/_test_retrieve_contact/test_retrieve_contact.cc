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
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/contact_invitation.h"
#include "demoweb_service/demoweb/module/baseline_user.h"
#include "demoweb_service/demoweb/module/retrieve_contact.h"
#include "message_queue/publisher/publisher.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_relation.pb.h"

int main() {
    e8::BeginTestSuite("retrieve_contact");
    e8::EndTestSuite();
    return 0;
}
