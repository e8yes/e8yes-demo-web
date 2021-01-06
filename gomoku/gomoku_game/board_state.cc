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

#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "gomoku/gomoku_game/board_state.h"

namespace e8 {

PlayerSide OffensiveSide() { return PS_PLAYER_A; }

MovePosition::MovePosition(int8_t const x, int8_t const y) : x(x), y(y) {}

bool MovePosition::operator==(MovePosition const &other) const {
    return x == other.x && y == other.y;
}

GomokuAction::GomokuAction(MovePosition const &stone_pos) : stone_pos(stone_pos) {}
GomokuAction::GomokuAction(Swap2Decision const decision) : swap2_decision(decision) {}
GomokuAction::GomokuAction(StoneTypeDecision const decision) : stone_type_decision(decision) {}

GomokuActionRecord::GomokuActionRecord(GamePhase const game_phase, PlayerSide const side,
                                       GomokuActionId const action_id, GomokuAction const &action)
    : game_phase(game_phase), side(side), action(std::make_pair(action_id, action)) {}

GomokuBoardState::GomokuBoardState(int16_t const width, int16_t const height)
    : width_(width), height_(height), game_result_(GameResult::GR_UNDETERMINED),
      current_game_phase_(GP_PLACE_3_STONES), current_player_side_(OffensiveSide()),
      board_(std::unique_ptr<StoneState[]>(new StoneState[width * height])),
      player_stone_type_({std::optional<StoneType>(StoneType::ST_BLACK), std::nullopt}),
      standard_gomoku_legal_actions_(2 * width * height) {
    for (int16_t y = 0; y < this->Height(); ++y) {
        for (int16_t x = 0; x < this->Width(); ++x) {
            MovePosition pos(x, y);
            GomokuActionId id = this->MovePositionToActionId(pos);
            standard_gomoku_legal_actions_.insert(std::make_pair(id, GomokuAction(pos)));
        }
    }

    swap2_decision_legal_actions_.insert(
        std::make_pair(this->Swap2DecisionToActionId(Swap2Decision::SW2D_CHOOSE_WHITE),
                       GomokuAction(Swap2Decision::SW2D_CHOOSE_WHITE)));
    swap2_decision_legal_actions_.insert(
        std::make_pair(this->Swap2DecisionToActionId(Swap2Decision::SW2D_CHOOSE_BLACK),
                       GomokuAction(Swap2Decision::SW2D_CHOOSE_BLACK)));
    swap2_decision_legal_actions_.insert(
        std::make_pair(this->Swap2DecisionToActionId(Swap2Decision::SW2D_PLACE_2_STONES),
                       GomokuAction(Swap2Decision::SW2D_PLACE_2_STONES)));

    stone_type_decision_legal_actions_.insert(
        std::make_pair(this->StoneTypeDecisionToActionId(StoneTypeDecision::STD_CHOOSE_WHITE),
                       GomokuAction(StoneTypeDecision::STD_CHOOSE_WHITE)));
    stone_type_decision_legal_actions_.insert(
        std::make_pair(this->StoneTypeDecisionToActionId(StoneTypeDecision::STD_CHOOSE_BLACK),
                       GomokuAction(StoneTypeDecision::STD_CHOOSE_BLACK)));
}

GomokuBoardState::GomokuBoardState(GomokuBoardState const &other)
    : GomokuBoardState(other.width_, other.height_) {
    game_result_ = other.game_result_;
    current_game_phase_ = other.current_game_phase_;
    current_player_side_ = other.current_player_side_;

    player_stone_type_ = other.player_stone_type_;
    for (int16_t i = 0; i < this->Width() * this->Height(); ++i) {
        board_[i] = other.board_[i];
    }

    swap2_decision_legal_actions_ = other.swap2_decision_legal_actions_;
    stone_type_decision_legal_actions_ = other.stone_type_decision_legal_actions_;
    standard_gomoku_legal_actions_ = other.standard_gomoku_legal_actions_;
}

std::unordered_map<GomokuActionId, GomokuAction> const &GomokuBoardState::LegalActions() const {
    switch (this->CurrentGamePhase()) {
    case GP_PLACE_3_STONES:
    case GP_SWAP2_PLACE_2_STONES:
    case GP_STANDARD_GOMOKU:
        return standard_gomoku_legal_actions_;
    case GP_SWAP2_DECISION:
        return swap2_decision_legal_actions_;
    case GP_STONE_TYPE_DECISION:
        return stone_type_decision_legal_actions_;
    }
    assert(false);
}

GomokuActionId GomokuBoardState::MovePositionToActionId(MovePosition const &pos) const {
    return pos.x + pos.y * this->Width();
}

GomokuActionId GomokuBoardState::Swap2DecisionToActionId(Swap2Decision const decision) const {
    return this->Width() * this->Height() + decision;
}

GomokuActionId
GomokuBoardState::StoneTypeDecisionToActionId(StoneTypeDecision const decision) const {
    return this->Width() * this->Height() + 3 + decision;
}

PlayerSide GomokuBoardState::CurrentPlayerSide() const { return current_player_side_; }

std::optional<StoneType> GomokuBoardState::PlayerStoneType(PlayerSide const player_side) const {
    return player_stone_type_[player_side];
}

GamePhase GomokuBoardState::CurrentGamePhase() const { return current_game_phase_; }

GameResult GomokuBoardState::ApplyAction(GomokuActionId const action_id,
                                         std::optional<GameResult> const cached_game_result) {
    assert(game_result_ == GameResult::GR_UNDETERMINED);

    std::unordered_map<GomokuActionId, GomokuAction>::const_iterator action_it;
    switch (this->CurrentGamePhase()) {
    case GP_PLACE_3_STONES:
    case GP_SWAP2_PLACE_2_STONES:
    case GP_STANDARD_GOMOKU: {
        action_it = standard_gomoku_legal_actions_.find(action_id);
        assert(action_it != standard_gomoku_legal_actions_.end());
        break;
    }
    case GP_SWAP2_DECISION: {
        action_it = swap2_decision_legal_actions_.find(action_id);
        assert(action_it != swap2_decision_legal_actions_.end());
        break;
    }
    case GP_STONE_TYPE_DECISION: {
        action_it = stone_type_decision_legal_actions_.find(action_id);
        assert(action_it != stone_type_decision_legal_actions_.end());
        break;
    }
    default:
        assert(false);
        break;
    }

    history_.push_back(GomokuActionRecord(this->CurrentGamePhase(), this->CurrentPlayerSide(),
                                          action_it->first, action_it->second));

    switch (this->CurrentGamePhase()) {
    case GP_PLACE_3_STONES: {
        assert(player_stone_type_[this->CurrentPlayerSide()].has_value());
        *this->ChessPieceStateAt(*action_it->second.stone_pos) =
            StoneState(*player_stone_type_[this->CurrentPlayerSide()]);

        if (history_.size() == 2) {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = StoneType::ST_WHITE;
        } else if (history_.size() == 3) {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = std::nullopt;
            current_game_phase_ = GamePhase::GP_SWAP2_DECISION;
            current_player_side_ = PlayerSide::PS_PLAYER_B;
        }
        break;
    }
    case GP_SWAP2_DECISION: {
        switch (*action_it->second.swap2_decision) {
        case SW2D_CHOOSE_WHITE: {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = StoneType::ST_BLACK;
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_WHITE;
            current_game_phase_ = GamePhase::GP_STANDARD_GOMOKU;
            break;
        }
        case SW2D_CHOOSE_BLACK: {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = StoneType::ST_WHITE;
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_BLACK;
            current_game_phase_ = GamePhase::GP_STANDARD_GOMOKU;
            current_player_side_ = PlayerSide::PS_PLAYER_A;
            break;
        }
        case SW2D_PLACE_2_STONES: {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_WHITE;
            current_game_phase_ = GamePhase::GP_SWAP2_PLACE_2_STONES;
            break;
        }
        default: {
            assert(false);
            break;
        }
        }
        break;
    }
    case GP_SWAP2_PLACE_2_STONES: {
        assert(player_stone_type_[this->CurrentPlayerSide()].has_value());
        *this->ChessPieceStateAt(*action_it->second.stone_pos) =
            StoneState(*player_stone_type_[this->CurrentPlayerSide()]);

        if (history_.size() == 5) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_BLACK;
        } else if (history_.size() == 6) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = std::nullopt;
            current_game_phase_ = GamePhase::GP_STONE_TYPE_DECISION;
            current_player_side_ = PlayerSide::PS_PLAYER_A;
        }
        break;
    }
    case GP_STONE_TYPE_DECISION: {
        switch (*action_it->second.stone_type_decision) {
        case STD_CHOOSE_WHITE: {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = StoneType::ST_WHITE;
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_BLACK;
            break;
        }
        case STD_CHOOSE_BLACK: {
            player_stone_type_[PlayerSide::PS_PLAYER_A] = StoneType::ST_BLACK;
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_WHITE;
            current_player_side_ = PlayerSide::PS_PLAYER_B;
            break;
        }
        }
        current_game_phase_ = GamePhase::GP_STANDARD_GOMOKU;
        break;
    }
    case GP_STANDARD_GOMOKU: {
        assert(player_stone_type_[this->CurrentPlayerSide()].has_value());
        *this->ChessPieceStateAt(*action_it->second.stone_pos) =
            StoneState(*player_stone_type_[this->CurrentPlayerSide()]);

        if (cached_game_result.has_value()) {
            game_result_ = *cached_game_result;
        } else {
            unsigned max_connected_stones = this->MaxConnectedStonesFrom(
                *action_it->second.stone_pos, *player_stone_type_[this->CurrentPlayerSide()]);
            if (max_connected_stones == 5) {
                switch (current_player_side_) {
                case PS_PLAYER_A:
                    game_result_ = GameResult::GR_PLAYER_A_WIN;
                    break;
                case PS_PLAYER_B:
                    game_result_ = GameResult::GR_PLAYER_B_WIN;
                    break;
                }
            } else if (max_connected_stones > 5 || this->LegalActions().empty()) {
                game_result_ = GameResult::GR_TIE;
            }
        }

        current_player_side_ =
            static_cast<PlayerSide>((static_cast<unsigned>(current_player_side_) + 1) | 1);

        standard_gomoku_legal_actions_.erase(action_it);
        break;
    }
    }

    return this->CurrentGameResult();
}

std::optional<GomokuActionRecord> GomokuBoardState::RetractAction() {
    if (history_.empty()) {
        return std::nullopt;
    }

    game_result_ = GameResult::GR_UNDETERMINED;

    GomokuActionRecord record = history_.back();

    switch (record.game_phase) {
    case GP_PLACE_3_STONES: {
        current_game_phase_ = GP_PLACE_3_STONES;
        current_player_side_ = PlayerSide::PS_PLAYER_A;
        if (history_.size() == 3) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_WHITE;
        } else if (history_.size() == 2) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_BLACK;
        }
        break;
    }
    case GP_SWAP2_DECISION: {
        current_game_phase_ = GP_SWAP2_DECISION;
        current_player_side_ = PlayerSide::PS_PLAYER_B;
        break;
    }
    case GP_SWAP2_PLACE_2_STONES: {
        current_game_phase_ = GP_SWAP2_PLACE_2_STONES;
        current_player_side_ = PlayerSide::PS_PLAYER_B;
        if (history_.size() == 6) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_BLACK;
        } else if (history_.size() == 5) {
            player_stone_type_[PlayerSide::PS_PLAYER_B] = StoneType::ST_WHITE;
        }
        break;
    }
    case GP_STONE_TYPE_DECISION: {
        current_game_phase_ = GP_STONE_TYPE_DECISION;
        current_player_side_ = PlayerSide::PS_PLAYER_A;
        break;
    }
    case GP_STANDARD_GOMOKU: {
        *this->ChessPieceStateAt(*record.action.second.stone_pos) = StoneState();
        standard_gomoku_legal_actions_.insert(record.action);
        current_player_side_ =
            static_cast<PlayerSide>((static_cast<unsigned>(current_player_side_) + 1) | 1);
        game_result_ = GameResult::GR_UNDETERMINED;
        break;
    }
    }

    history_.pop_back();

    return record;
}

std::optional<GomokuActionRecord> GomokuBoardState::LastAction() const {
    if (history_.empty()) {
        return std::nullopt;
    }
    return history_.back();
}

GameResult GomokuBoardState::CurrentGameResult() const { return game_result_; }

StoneState *GomokuBoardState::CurrentBoard() const { return board_.get(); }

std::vector<GomokuActionRecord> const &GomokuBoardState::History() const { return history_; }

int16_t GomokuBoardState::Width() const { return width_; }

int16_t GomokuBoardState::Height() const { return height_; }

StoneState *GomokuBoardState::ChessPieceStateAt(MovePosition const &pos) {
    return &board_[pos.x + pos.y * width_];
}

StoneState const *GomokuBoardState::ChessPieceStateAt(MovePosition const &pos) const {
    return &board_[pos.x + pos.y * width_];
}

uint8_t GomokuBoardState::MaxConnectedStonesFrom(MovePosition const &move_pos,
                                                 StoneType const stone_type) const {
    int8_t max_connected_stones = 0;

    // Counter clockwise 135 degrees.
    MovePosition pos(move_pos.x - 1, move_pos.y - 1);
    while (pos.x >= 0 && pos.y >= 0 && *this->ChessPieceStateAt(pos) == stone_type) {
        --pos.x;
        --pos.y;
    }
    if (move_pos.x - pos.x - 1 > max_connected_stones) {
        max_connected_stones = move_pos.x - pos.x - 1;
    }

    // Counter clockwise 90 degrees.
    pos.x = move_pos.x;
    pos.y = move_pos.y - 1;
    while (pos.y >= 0 && *this->ChessPieceStateAt(pos) == stone_type) {
        --pos.y;
    }
    if (move_pos.y - pos.y - 1 > max_connected_stones) {
        max_connected_stones = move_pos.y - pos.y - 1;
    }

    // Counter clockwise 45 degrees.
    pos.x = move_pos.x + 1;
    pos.y = move_pos.y - 1;
    while (pos.x < width_ && pos.y >= 0 && *this->ChessPieceStateAt(pos) == stone_type) {
        ++pos.x;
        --pos.y;
    }
    if (move_pos.y - pos.y - 1 > max_connected_stones) {
        max_connected_stones = move_pos.y - pos.y - 1;
    }

    // Counter clockwise 180 degrees.
    pos.x = move_pos.x - 1;
    pos.y = move_pos.y;
    while (pos.x >= 0 && *this->ChessPieceStateAt(pos) == stone_type) {
        --pos.x;
    }
    if (move_pos.x - pos.x - 1 > max_connected_stones) {
        max_connected_stones = move_pos.x - pos.x - 1;
    }

    // Counter clockwise 0 degrees.
    pos.x = move_pos.x + 1;
    pos.y = move_pos.y;
    while (pos.x < width_ && *this->ChessPieceStateAt(pos) == stone_type) {
        ++pos.x;
    }
    if (pos.x - move_pos.x - 1 > max_connected_stones) {
        max_connected_stones = pos.x - move_pos.x - 1;
    }

    // Counter clockwise -135 degrees.
    pos.x = move_pos.x - 1;
    pos.y = move_pos.y + 1;
    while (pos.x >= 0 && pos.y < height_ && *this->ChessPieceStateAt(pos) == stone_type) {
        --pos.x;
        ++pos.y;
    }
    if (move_pos.x - pos.x - 1 > max_connected_stones) {
        max_connected_stones = move_pos.x - pos.x - 1;
    }

    // Counter clockwise -90 degrees.
    pos.x = move_pos.x;
    pos.y = move_pos.y + 1;
    while (pos.y < height_ && *this->ChessPieceStateAt(pos) == stone_type) {
        ++pos.y;
    }
    if (pos.y - move_pos.y - 1 > max_connected_stones) {
        max_connected_stones = pos.y - move_pos.y - 1;
    }

    // Counter clockwise -45 degrees.
    pos.x = move_pos.x + 1;
    pos.y = move_pos.y + 1;
    while (pos.x < width_ && pos.y < height_ && *this->ChessPieceStateAt(pos) == stone_type) {
        ++pos.x;
        ++pos.y;
    }
    if (pos.y - move_pos.y - 1 > max_connected_stones) {
        max_connected_stones = pos.y - move_pos.y - 1;
    }

    assert(max_connected_stones >= 0);
    return max_connected_stones;
}

} // namespace e8
