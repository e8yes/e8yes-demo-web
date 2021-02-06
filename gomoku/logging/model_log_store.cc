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

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "common/time_util/time_util.h"
#include "gomoku/game/board_state.h"
#include "gomoku/logging/common_types.h"
#include "gomoku/logging/model_log_store.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

char const *kGomokuModelIdSeqTableName = "gomoku_model_id_seq";
char const *kGomokuModelTableName = "gomoku_model";

} // namespace

GomokuModelEntity::GomokuModelEntity()
    : SqlEntityInterface({&id, &model_name, &model_path, &created_at, &policy_test_cross_entropy,
                          &value_test_mse, &total_test_loss, &stats_updated_at}) {}

GomokuModelEntity::GomokuModelEntity(GomokuModelEntity const &other) : GomokuModelEntity() {
    id = other.id;
    model_name = other.model_name;
    model_path = other.model_path;
    created_at = other.created_at;
    policy_test_cross_entropy = other.policy_test_cross_entropy;
    value_test_mse = other.value_test_mse;
    total_test_loss = other.total_test_loss;
    stats_updated_at = other.stats_updated_at;
}

GomokuModelEntity &GomokuModelEntity::operator=(GomokuModelEntity const &other) {
    id = other.id;
    model_name = other.model_name;
    model_path = other.model_path;
    created_at = other.created_at;
    policy_test_cross_entropy = other.policy_test_cross_entropy;
    value_test_mse = other.value_test_mse;
    total_test_loss = other.total_test_loss;
    stats_updated_at = other.stats_updated_at;
    return *this;
}

ModelLogStore::ModelLogStore(ConnectionReservoirInterface *conns) : conns_(conns) {}

std::optional<GomokuModelEntity> ModelLogStore::LastModel() {
    SqlQueryBuilder query;
    query.QueryPiece(kGomokuModelTableName).QueryPiece(" gm ORDER BY gm.id DESC LIMIT 1");

    std::vector<std::tuple<GomokuModelEntity>> query_result =
        Query<GomokuModelEntity>(query, {"gm"}, conns_);
    if (query_result.empty()) {
        return std::nullopt;
    }

    assert(query_result.size() == 1);
    return std::get<0>(query_result[0]);
}

GomokuModelEntity ModelLogStore::LogNewModel(std::string const &model_name,
                                             std::string const &model_path) {
    GomokuModelEntity entity;
    *entity.id.ValuePtr() = SeqId(kGomokuModelIdSeqTableName, conns_);
    *entity.model_name.ValuePtr() = model_name;
    *entity.model_path.ValuePtr() = model_path;
    *entity.created_at.ValuePtr() = CurrentTimestampMicros();
    *entity.stats_updated_at.ValuePtr() = CurrentTimestampMicros();

    uint64_t num_rows = Update(entity, kGomokuModelTableName, /*replace=*/false, conns_);
    assert(num_rows == 1);

    return entity;
}

void ModelLogStore::LogNewStats(ModelId model_id, float policy_test_cross_entropy,
                                float value_test_mse, float total_test_loss) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> model_id_ph;
    query.QueryPiece(kGomokuModelTableName).QueryPiece(" gm WHERE gm.id=").Holder(&model_id_ph);
    query.SetValueToPlaceholder(model_id_ph, std::make_shared<SqlLong>(model_id));

    std::vector<std::tuple<GomokuModelEntity>> query_result =
        Query<GomokuModelEntity>(query, {"gm"}, conns_);
    assert(query_result.size() == 1);

    GomokuModelEntity entity = std::get<0>(query_result[0]);
    *entity.policy_test_cross_entropy.ValuePtr() = policy_test_cross_entropy;
    *entity.value_test_mse.ValuePtr() = value_test_mse;
    *entity.total_test_loss.ValuePtr() = total_test_loss;
    *entity.stats_updated_at.ValuePtr() = CurrentTimestampMicros();

    uint64_t num_rows = Update(entity, kGomokuModelTableName, /*replace=*/true, conns_);
    assert(num_rows == 1);
}

} // namespace e8
