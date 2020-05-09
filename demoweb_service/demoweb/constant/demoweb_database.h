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

#ifndef TABLE_NAMES_H
#define TABLE_NAMES_H

#include <string>

namespace e8 {

static char const kDemowebDatabaseName[] = "demoweb";

/**
 * @brief The TableNames struct Table name constants for the demoweb schema.
 */
struct TableNames {
    static std::string AUser() { return "auser"; }
};

} // namespace e8

#endif // TABLE_NAMES_H
