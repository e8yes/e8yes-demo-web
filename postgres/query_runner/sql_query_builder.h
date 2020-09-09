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
#ifndef SQL_QUERY_BUILDER_H
#define SQL_QUERY_BUILDER_H

#include <memory>
#include <string>
#include <vector>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/reflection/sql_primitive_interface.h"

namespace e8 {

/**
 * @brief The SqlQueryBuilder class Builder class to construct a parameterized SQL query from query
 * string pieces and variable placeholders.
 */
class SqlQueryBuilder {
  public:
    SqlQueryBuilder() = default;
    ~SqlQueryBuilder() = default;
    SqlQueryBuilder(SqlQueryBuilder const &) = default;

    /**
     * @brief Append a query string piece.
     *
     * @param piece Query string piece
     * @return The current builder.
     */
    SqlQueryBuilder &QueryPiece(std::string const &piece);

    /**
     * @brief Represents a variable placeholder
     *
     * @param <Type> The type of variable the place holder represents for.
     */
    template <typename Type> class Placeholder {
      public:
        std::vector<ConnectionInterface::QueryParams::SlotId> param_slots;

        /**
         * @brief Clear reset the placeholder.
         */
        void Clear() { param_slots.clear(); }
    };

    /**
     * @brief Append a variable placeholder.
     *
     * @param holder Variable placeholder
     * @return The current builder.
     */
    template <typename Type> SqlQueryBuilder &Holder(Placeholder<Type> *holder) {
        ConnectionInterface::QueryParams::SlotId slot_id = params_.AllocateSlot();
        holder->param_slots.push_back(slot_id);
        query_ += "$" + std::to_string(slot_id);
        return *this;
    }

    /**
     * @brief Collectively assigns the value to the holder to all the query positions.
     *
     * @param <Type>
     * @param holder Placeholder to be assigned a value.
     * @param val Pointer to the value to be assigned.
     */
    template <typename Type>
    void SetValueToPlaceholder(Placeholder<Type> const &holder,
                               std::shared_ptr<SqlPrimitiveInterface> const &val) {
        for (auto slot : holder.param_slots) {
            params_.SetParam(slot, val);
        }
    }

    /**
     * @brief Export a postgres-compliant query from the previously appended pieces.
     *
     * @return postgres-compliant query
     */
    std::string const &PsqlQuery() const;

    /**
     * @brief Export the assigned query parameters.
     *
     * @return The assigned query parameters
     */
    ConnectionInterface::QueryParams const &QueryParams() const;

  private:
    std::string query_;
    ConnectionInterface::QueryParams params_;
};

} // namespace e8

#endif // SQL_QUERY_BUILDER_H
