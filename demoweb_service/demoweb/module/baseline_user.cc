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
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "demoweb_service/demoweb/module/baseline_user.h"
#include "demoweb_service/demoweb/module/system_user_group.h"
#include "demoweb_service/demoweb/module/user_identity.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

std::optional<UserEntity> CreateBaselineUser(std::string const &security_key,
                                             std::optional<UserId> user_id, HostId const host_id,
                                             ConnectionReservoirInterface *db_conn) {
    return CreateUser(
        security_key,
        std::vector<std::string>({kSystemUserGroupStrings[SystemUserGroup::BASELINE_USER_GROUP]}),
        user_id, host_id, db_conn);
}

} // namespace e8
