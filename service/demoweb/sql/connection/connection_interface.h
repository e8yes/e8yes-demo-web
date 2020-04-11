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

#include <memory> // IWYU pragma: keep
#include <stdint.h>
// IWYU pragma: no_include <bits/stdint-intn.h>
#include <string>
#include <unordered_map>

#include "sql/reflection/sql_primitive_interface.h"
#include "sql/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The ConnectionInterface class Represents a connection to a database server.
 */
class ConnectionInterface {
  public:
    ConnectionInterface() = default;
    virtual ~ConnectionInterface() = default;
    ConnectionInterface(ConnectionInterface const &) = delete;

    /**
     * @brief Stores information about the parameter values to a parameterized query
     */
    class QueryParams {
      public:
        using SlotId = int32_t;

        /**
         * @brief Clear all the parameter values.
         */
        void clear() { params_.clear(); }

        /**
         * @brief Set value to the position-th parameter placeholder.
         *
         * @param position Position to set value to.
         * @param val value to set.
         */
        void set_param(SlotId slot, SqlPrimitiveInterface const &val) {
            params_.insert(std::make_pair(slot, &val));
        }

        /**
         * @brief Get the value set to the position-th placeholder.
         *
         * @param position Position to read from.
         * @return The value if it exists. Otherwise null.
         */
        SqlPrimitiveInterface const *get_param(SlotId slot) const {
            auto it = params_.find(slot);
            if (it != params_.end()) {
                return it->second;
            } else {
                return nullptr;
            }
        }

        /**
         * @brief Get the number of placeholders that are assigned a value of.
         *
         * @return The number of placeholders.
         */
        size_t num_slots() { return params_.size(); }

        /**
         * @brief allocate_slot Allocate a new parameter slot.
         *
         * @return ID of the parameter slot.
         */
        SlotId allocate_slot() { return ++next_slot_id_; }

      private:
        std::unordered_map<SlotId, SqlPrimitiveInterface const *> params_;
        SlotId next_slot_id_ = 0;
    };

    /**
     * Run a parameterized query.
     *
     * @param query Query to run.
     * @param params Parameters for the query.
     * @return Query's result set.
     */
    virtual std::unique_ptr<ResultSetInterface> run_query(std::string const &query,
                                                          QueryParams const &params) = 0;

    /**
     * @brief Run a parameterized update query.
     *
     * @param query Update query to run.
     * @param params Parameters for the query.
     * @return The number of rows updated by the query.
     */
    virtual int run_update(std::string const &query, QueryParams const &params) = 0;

    /**
     * @brief Closing the connection.
     */
    virtual void close() = 0;

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
