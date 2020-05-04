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

#ifndef RESULT_SET_INTERFACE_H
#define RESULT_SET_INTERFACE_H

#include "postgres/query_runner/reflection/sql_primitive_interface.h"

namespace e8 {

/**
 * @brief The ResultSetInterface class An iterator-like class to access the set of rows resulting
 * from a query.
 */
class ResultSetInterface {
  public:
    ResultSetInterface() = default;
    virtual ~ResultSetInterface() = default;

    /**
     * @brief Move cursor to the next record if possible.
     */
    virtual void next() = 0;

    /**
     * @brief Check if the cursor reaches the end of the result set.
     *
     * @return false when the cursor has moved to the end of the result set. Otherwise, it returns
     *     true.
     */
    virtual bool has_next() const = 0;

    /**
     * @brief Set the value of the ith cell at the current row cursor position to the record's
     * field.
     *
     * @param i The i-th(zero-offset) cell to pull value from.
     * @param field The field to assign value to.
     */
    virtual void set_field(unsigned i, SqlPrimitiveInterface *field) = 0;
};

} // namespace e8

#endif // RESULT_SET_INTERFACE_H
