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
#include <memory>
#include <optional>

#include "gomoku/game/board_state.h"
#include "gomoku/game/game.h"

namespace e8 {

GomokuGame::GomokuGame(std::shared_ptr<GomokuPlayerInterface> const &player_a,
                       std::shared_ptr<GomokuPlayerInterface> const &player_b)
    : player_a_(std::move(player_a)), player_b_(std::move(player_b)) {}

void GomokuGame::Start() {
    do {
        GomokuBoardState board(kWidth, kHeight);

        player_a_->OnGomokuGameBegin(board);
        player_b_->OnGomokuGameBegin(board);

        GameResult game_result;

        do {
            GomokuPlayerInterface *current_player;

            switch (board.CurrentPlayerSide()) {
            case PS_PLAYER_A: {
                current_player = player_a_.get();
                break;
            }
            case PS_PLAYER_B: {
                current_player = player_b_.get();
                break;
            }
            default: {
                assert(false);
                break;
            }
            }

            GomokuActionId action_id = current_player->NextPlayerAction(board);
            game_result = board.ApplyAction(action_id, /*cached_game_result=*/std::nullopt);

            player_a_->AfterGomokuActionApplied(board);
            player_b_->AfterGomokuActionApplied(board);

        } while (game_result == GameResult::GR_UNDETERMINED);

        player_a_->OnGameEnded(board);
        player_b_->OnGameEnded(board);

    } while (player_a_->WantAnotherGame() && player_b_->WantAnotherGame());
}

} // namespace e8
