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

#include "common/flags/parse_flags.h"

namespace e8 {
namespace flags_internal {

static std::vector<std::string> gArgv;

std::optional<std::string> ScanFlag(std::string const &flag) {
    std::string expected_flag_prefix = "--" + flag + "=";

    for (unsigned i = 1; i < gArgv.size(); i++) {
        std::string const &cur_flag = gArgv[i];
        if (cur_flag.rfind(expected_flag_prefix, 0) == 0) {
            std::string flag_value = cur_flag.substr(std::string(expected_flag_prefix).length());
            return flag_value;
        }
    }

    return std::nullopt;
}

} // namespace flags_internal

void Argv(int argc, char **argv) {
    flags_internal::gArgv.clear();
    for (int i = 0; i < argc; i++) {
        flags_internal::gArgv.push_back(argv[i]);
    }
}

} // namespace e8
