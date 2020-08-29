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

#include <cassert>
#include <regex>
#include <string>
#include <unistd.h>

#include "demoweb_service/demoweb/environment/host_id.h"

namespace e8 {
namespace {

/**
 * @brief ParseHostIdFromHostName host_name has the format [a-zA-Z_]*([0-9]+). The trailing
 * numeric part of the host name is the host ID.
 */
HostId ParseHostIdFromHostName(std::string const &host_name) {
    std::regex re(R"(^[a-zA-Z_]*([0-9]+)$)");
    std::smatch sm;
    std::regex_match(host_name, sm, re);
    assert(sm.size() == 2);
    return static_cast<unsigned>(std::stoi(sm[1].str()));
}

} // namespace

HostId CurrentHostId() {
    char host_name[64];
    gethostname(host_name, sizeof(host_name));
    return ParseHostIdFromHostName(host_name);
}

} // namespace e8
