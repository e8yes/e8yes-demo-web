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

#ifndef PQ_RESULT_SET_H
#define PQ_RESULT_SET_H

#include <memory>
#include <pqxx/result.hxx>
#include <pqxx/result_iterator.hxx>

#include "postgres/query_runner/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The PqResultSet class Implements the result set interface using the pq client.
 */
class PqResultSet : public ResultSetInterface {
  public:
    PqResultSet() = default;
    PqResultSet(pqxx::result const &rs);
    ~PqResultSet() override = default;
    PqResultSet(PqResultSet const &) = delete;

    void Next() override;
    bool HasNext() const override;
    void SetField(unsigned i, SqlPrimitiveInterface *field) override;

  private:
    pqxx::result rs_;
    pqxx::result::const_iterator it_;
};

} // namespace e8

#endif // PQ_RESULT_SET_H
