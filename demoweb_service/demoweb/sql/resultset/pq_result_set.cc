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

#include <pqxx/result.hxx>
#include <pqxx/row.hxx>

#include "sql/reflection/sql_primitive_interface.h"
#include "sql/resultset/pq_result_set.h"

namespace e8 {

PqResultSet::PqResultSet(pqxx::result const &rs) : rs_(rs) { it_ = rs_.begin(); }

void PqResultSet::next() { ++it_; }

bool PqResultSet::has_next() const { return it_ != rs_.end(); }

void PqResultSet::set_field(unsigned i, SqlPrimitiveInterface *field) {
    field->import_from_field((*it_)[i]);
}

} // namespace e8
