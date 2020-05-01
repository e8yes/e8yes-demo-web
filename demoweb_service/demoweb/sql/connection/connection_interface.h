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

#ifndef CONNECTIONINTERFACE_H
#define CONNECTIONINTERFACE_H

#include <cstdint> // IWYU pragma: no_include <bits/stdint-intn.h>
#include <map>
#include <memory> // IWYU pragma: keep
#include <string>

#include "sql/reflection/sql_primitive_interface.h"
#include "sql/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The ConnectionInterface class Represents a connection to a database server.
 * There should only be one thread accessing a connection at a time.
 */
class ConnectionInterface {
  public:
    ConnectionInterface() = default;
    virtual ~ConnectionInterface() = default;
    ConnectionInterface(ConnectionInterface const &) = delete;

    using ParameterizedQuery = std::string;

    /**
     * @brief Stores information about the parameter values to a parameterized query
     */
    class QueryParams {
      public:
        QueryParams() = default;
        QueryParams(QueryParams const &) = default;
        ~QueryParams() = default;

        using SlotId = uint32_t;

        /**
         * @brief Clear all the parameter values.
         */
        void clear();

        /**
         * @brief Set value to the position-th parameter placeholder.
         *
         * @param position Position to set value to.
         * @param val Pointer to the value to set.
         */
        void set_param(SlotId slot, SqlPrimitiveInterface const *val);

        /**
         * @brief Get the value set to the position-th placeholder.
         *
         * @param position Position to read from.
         * @return The value if it exists. Otherwise null.
         */
        SqlPrimitiveInterface const *get_param(SlotId slot) const;

        /**
         * @brief Get the number of placeholders that are assigned a value of.
         *
         * @return The number of placeholders.
         */
        size_t num_slots() const;

        /**
         * @brief allocate_slot Allocate a new parameter slot.
         *
         * @return ID of the parameter slot.
         */
        SlotId allocate_slot();

        /**
         * @brief params Returns the internally allocated parameters.
         *
         * @return Slot parameters.
         */
        std::map<SlotId, SqlPrimitiveInterface const *> const &parameters() const;

      private:
        std::map<SlotId, SqlPrimitiveInterface const *> params_;
        SlotId next_slot_id_ = 0;
    };

    /**
     * Run a parameterized query.
     *
     * @param query Query to run.
     * @param params Parameters for the query.
     * @return Query's result set.
     */
    virtual std::unique_ptr<ResultSetInterface> run_query(ParameterizedQuery const &query,
                                                          QueryParams const &params) = 0;

    /**
     * @brief Run a parameterized update query.
     *
     * @param query Update query to run.
     * @param params Parameters for the query.
     * @return The number of rows updated by the query.
     */
    virtual uint64_t run_update(ParameterizedQuery const &query, QueryParams const &params) = 0;

    /**
     * @brief Check if the connection is closed
     *
     * @throws java.sql.SQLException
     * @return true if it is closed.
     */
    virtual bool is_closed() const = 0;
};

} // namespace e8

#endif // CONNECTIONINTERFACE_H
