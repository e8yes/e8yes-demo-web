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

#include <string>

#include "sql/reflection/sql_primitive_interface.h"

namespace e8 {

SqlPrimitiveInterface::SqlPrimitiveInterface(std::string const &field_name)
    : field_name_(field_name) {}

SqlPrimitiveInterface::~SqlPrimitiveInterface() {}

std::string const &SqlPrimitiveInterface::field_name() const { return field_name_; }

} // namespace e8
