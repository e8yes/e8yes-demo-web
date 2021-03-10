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

#ifndef MODEL_LOG_STORE_H
#define MODEL_LOG_STORE_H

#include <optional>
#include <string>

#include "gomoku/logging/common_types.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {

/**
 * @brief The GomokuModelEntity struct C++ class that represents a record from the sql table
 * gomoku_model.
 */
struct GomokuModelEntity : public SqlEntityInterface {
    GomokuModelEntity();
    GomokuModelEntity(GomokuModelEntity const &other);
    ~GomokuModelEntity() = default;

    GomokuModelEntity &operator=(GomokuModelEntity const &other);

    SqlLong id = SqlLong("id");
    SqlStr model_name = SqlStr("model_name");
    SqlStr model_path = SqlStr("model_path");
    SqlTimestamp created_at = SqlTimestamp("created_at");
    SqlFloat policy_test_cross_entropy = SqlFloat("policy_test_cross_entropy");
    SqlFloat value_test_mse = SqlFloat("value_test_mse");
    SqlFloat total_test_loss = SqlFloat("total_test_loss");
    SqlTimestamp stats_updated_at = SqlTimestamp("stats_updated_at");
};

/**
 * @brief The ModelLogStore class Handles model metadata logging. The logs will be written to the
 * database.
 */
class ModelLogStore {
  public:
    /**
     * @brief ModelLogStore All the logs will be written to the specified connection reservoir
     * target.
     */
    explicit ModelLogStore(ConnectionReservoirInterface *conns);

    /**
     * @brief LastModel Get the last created model if there is any.
     */
    std::optional<GomokuModelEntity> LastModel();

    /**
     * @brief LogNewModel Creates a new model entry and initializes its stats of nulls. The stats
     * can be updated afterwards using the function LogNewStats().
     *
     * @param model_name Name of the model.
     * @param model_path Path to the model file.
     * @return The logged gomoku model entity.
     */
    GomokuModelEntity LogNewModel(std::string const &model_name, std::string const &model_path);

    /**
     * @brief LogNewStats Update the stats to an existing model.
     *
     * @param model_id ID of the existing model whose stats needs to be updated.
     */
    void LogNewStats(ModelId model_id, float policy_test_cross_entropy, float value_test_mse,
                     float total_test_loss);

  private:
    ConnectionReservoirInterface *const conns_;
};

} // namespace e8

#endif // MODEL_LOG_STORE_H
