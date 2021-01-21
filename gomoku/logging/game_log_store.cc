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
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "common/time_util/time_util.h"
#include "gomoku/game/board_state.h"
#include "gomoku/logging/game_log_store.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_entity_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {
namespace {

char const *kGomokuGameIdSeqTableName = "gomoku_game_id_seq";
char const *kGomokuTableName = "gomoku_game";
char const *kGomokuActionTableName = "gomoku_game_action";

struct GomokuGameEntity : public SqlEntityInterface {
    GomokuGameEntity()
        : SqlEntityInterface({&id, &game_purpose, &player_a_id, &player_b_id, &player_a_model_id,
                              &player_b_model_id, &game_result, &created_at, &end_at}) {}

    GomokuGameEntity(GomokuGameEntity const &other) : GomokuGameEntity() {
        id = other.id;
        game_purpose = other.game_purpose;
        player_a_id = other.player_a_id;
        player_b_id = other.player_b_id;
        player_a_model_id = other.player_a_model_id;
        player_b_model_id = other.player_b_model_id;
        game_result = other.game_result;
        created_at = other.created_at;
        end_at = other.end_at;
    }

    GomokuGameEntity &operator=(GomokuGameEntity const &other) {
        id = other.id;
        game_purpose = other.game_purpose;
        player_a_id = other.player_a_id;
        player_b_id = other.player_b_id;
        player_a_model_id = other.player_a_model_id;
        player_b_model_id = other.player_b_model_id;
        game_result = other.game_result;
        created_at = other.created_at;
        end_at = other.end_at;
        return *this;
    }

    SqlLong id = SqlLong("id");
    SqlInt game_purpose = SqlInt("game_purpose");
    SqlLong player_a_id = SqlLong("player_a_id");
    SqlLong player_b_id = SqlLong("player_b_id");
    SqlLong player_a_model_id = SqlLong("player_a_model_id");
    SqlLong player_b_model_id = SqlLong("player_b_model_id");
    SqlInt game_result = SqlInt("game_result");
    SqlTimestamp created_at = SqlTimestamp("created_at");
    SqlTimestamp end_at = SqlTimestamp("end_at");
};

struct GomokuGameActionEntity : public SqlEntityInterface {
    GomokuGameActionEntity()
        : SqlEntityInterface({&game_id, &step_number, &action_number, &action_performed_by_player,
                              &action_stone_type, &board_before, &game_phase, &stochastic_policy,
                              &final_value, &created_at}) {}

    GomokuGameActionEntity(GomokuGameActionEntity const &other) : GomokuGameActionEntity() {
        game_id = other.game_id;
        step_number = other.step_number;
        action_number = other.action_number;
        action_performed_by_player = other.action_performed_by_player;
        action_stone_type = other.action_stone_type;
        board_before = other.board_before;
        game_phase = other.game_phase;
        stochastic_policy = other.stochastic_policy;
        final_value = other.final_value;
        created_at = other.created_at;
    }

    GomokuGameActionEntity &operator=(GomokuGameActionEntity const &other) {
        game_id = other.game_id;
        step_number = other.step_number;
        action_number = other.action_number;
        action_performed_by_player = other.action_performed_by_player;
        action_stone_type = other.action_stone_type;
        board_before = other.board_before;
        game_phase = other.game_phase;
        stochastic_policy = other.stochastic_policy;
        final_value = other.final_value;
        created_at = other.created_at;
        return *this;
    }

    SqlLong game_id = SqlLong("game_id");
    SqlInt step_number = SqlInt("step_number");
    SqlInt action_number = SqlInt("action_number");
    SqlInt action_performed_by_player = SqlInt("action_performed_by_player");
    SqlInt action_stone_type = SqlInt("action_stone_type");
    SqlByteArr board_before = SqlByteArr("board_before");
    SqlInt game_phase = SqlInt("game_phase");
    SqlFloatArr stochastic_policy = SqlFloatArr("stochastic_policy");
    SqlFloat final_value = SqlFloat("final_value");
    SqlTimestamp created_at = SqlTimestamp("created_at");
};

std::optional<GomokuGameEntity> FetchGame(GameId game_id, ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> game_id_ph;
    query.QueryPiece(kGomokuTableName).QueryPiece(" g WHERE g.id=").Holder(&game_id_ph);
    query.SetValueToPlaceholder(game_id_ph, std::make_shared<SqlLong>(game_id));

    std::vector<std::tuple<GomokuGameEntity>> query_result =
        Query<GomokuGameEntity>(query, {"g"}, conns);
    if (query_result.empty()) {
        return std::nullopt;
    }
    assert(query_result.size() == 1);

    return std::get<0>(query_result[0]);
}

std::optional<GomokuGameActionEntity> FetchGameAction(GameId game_id, GameStepNumber step_number,
                                                      ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> game_id_ph;
    SqlQueryBuilder::Placeholder<SqlInt> step_number_ph;
    query.QueryPiece(kGomokuActionTableName)
        .QueryPiece(" ga WHERE ga.game_id=")
        .Holder(&game_id_ph)
        .QueryPiece(" AND ga.step_number=")
        .Holder(&step_number_ph);

    query.SetValueToPlaceholder(game_id_ph, std::make_shared<SqlLong>(game_id));
    query.SetValueToPlaceholder(step_number_ph, std::make_shared<SqlInt>(step_number));

    std::vector<std::tuple<GomokuGameActionEntity>> query_result =
        Query<GomokuGameActionEntity>(query, {"ga"}, conns);
    if (query_result.empty()) {
        return std::nullopt;
    }
    assert(query_result.size() == 1);

    return std::get<0>(query_result[0]);
}

std::string ExtractBoardFeatures(GomokuBoardState const &board) {
    std::string board_features;
    for (int16_t y = 0; y < board.Height(); ++y) {
        for (int16_t x = 0; x < board.Width(); ++x) {
            board_features += *board.ChessPieceStateAt(MovePosition(x, y));
        }
    }
    return board_features;
}

} // namespace

GameLogStore::GameLogStore(ConnectionReservoirInterface *conns) : conns_(conns) {}

GameId GameLogStore::LogNewGeneratorGame(GameLogPurpose game_purpose,
                                         std::optional<ModelId> player_a_model_id,
                                         std::optional<ModelId> player_b_model_id) {
    GomokuGameEntity entity;
    *entity.id.ValuePtr() = SeqId(kGomokuGameIdSeqTableName, conns_);
    *entity.game_purpose.ValuePtr() = game_purpose;
    *entity.player_a_model_id.ValuePtr() = player_a_model_id;
    *entity.player_b_model_id.ValuePtr() = player_b_model_id;
    *entity.game_result.ValuePtr() = GameResult::GR_UNDETERMINED;

    uint64_t num_rows = Update(entity, kGomokuTableName, /*replace=*/false, conns_);
    assert(num_rows == 1);

    return *entity.id.Value();
}

void GameLogStore::LogGameAction(GameId game_id, GameStepNumber step_number,
                                 GomokuActionId action_number,
                                 PlayerSide action_performed_by_player, StoneType action_stone_type,
                                 GomokuBoardState const &board_before, GamePhase game_phase,
                                 std::vector<float> const &stochastic_policy) {
    GomokuGameActionEntity entity;
    *entity.game_id.ValuePtr() = game_id;
    *entity.step_number.ValuePtr() = step_number;
    *entity.action_number.ValuePtr() = action_number;
    *entity.action_performed_by_player.ValuePtr() = action_performed_by_player;
    *entity.action_stone_type.ValuePtr() = action_stone_type;
    *entity.board_before.ValuePtr() = ExtractBoardFeatures(board_before);
    *entity.game_phase.ValuePtr() = game_phase;
    *entity.stochastic_policy.ValuePtr() = stochastic_policy;

    uint64_t num_rows = Update(entity, kGomokuActionTableName, /*replace=*/false, conns_);
    assert(num_rows == 1);
}

void GameLogStore::LogGameActionValue(GameId game_id, GameStepNumber step_number,
                                      float final_value) {
    std::optional<GomokuGameActionEntity> game_action =
        FetchGameAction(game_id, step_number, conns_);
    assert(game_action.has_value());

    *game_action->final_value.ValuePtr() = final_value;

    Update(*game_action, kGomokuActionTableName, /*replace=*/true, conns_);
}

void GameLogStore::LogGameEnd(GameId game_id, GameResult game_result) {
    std::optional<GomokuGameEntity> game = FetchGame(game_id, conns_);
    assert(game.has_value());

    *game->game_result.ValuePtr() = game_result;
    *game->end_at.ValuePtr() = CurrentTimestampMicros();

    Update(*game, kGomokuTableName, /*replace=*/true, conns_);
}

} // namespace e8
