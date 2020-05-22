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

#include <cassert>
#include <optional>
#include <string>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_identity/user_profile.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

bool UpdateProfile(std::optional<std::string> const &alias, UserEntity *user,
                   ConnectionReservoirInterface *db_conns) {
    if (alias.has_value()) {
        *user->alias.value_ptr() = alias.value();
    }

    int num_rows_updated = Update(*user, TableNames::AUser(), /*override=*/true, db_conns);
    if (num_rows_updated == 0) {
        return false;
    }

    assert(num_rows_updated == 1);

    return true;
}

} // namespace e8
