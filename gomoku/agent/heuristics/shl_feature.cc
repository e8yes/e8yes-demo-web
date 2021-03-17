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

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/agent/heuristics/shl_feature.h"
#include "gomoku/agent/search/mct_node.h"
#include "gomoku/game/board_state.h"

namespace e8 {
namespace {

enum LinkPatternDfaState { COUNT_PIECE, PUNCTUATION };

std::tuple<int8_t, int8_t, bool> LinkageCountsAtWithDirection(int8_t x, int8_t y, int8_t dx,
                                                              int8_t dy,
                                                              StoneType compute_for_stone_type,
                                                              GomokuBoardState const &board) {
    int8_t raw_count = 0;
    int8_t holes = 0;
    bool blocked = false;
    int8_t i = 1;

    LinkPatternDfaState state = LinkPatternDfaState::COUNT_PIECE;

    while (true) {
        switch (state) {
        case COUNT_PIECE: {
            if (i > 6) {
                return std::make_tuple(raw_count, holes, blocked);
            }

            int8_t shifted_x = x + i * dx;
            int8_t shifted_y = y + i * dy;

            if (shifted_x < 0 || shifted_x >= board.Width() || shifted_y < 0 ||
                shifted_y >= board.Height() ||
                (*board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) !=
                     StoneType::ST_NONE &&
                 *board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) !=
                     compute_for_stone_type)) {
                blocked = true;
                return std::make_tuple(raw_count, holes, blocked);
            } else if (*board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) ==
                       StoneType::ST_NONE) {
                state = LinkPatternDfaState::PUNCTUATION;
            } else {
                ++raw_count;
                ++i;
            }
            break;
        }
        case PUNCTUATION: {
            ++i;

            if (i > 6) {
                return std::make_tuple(raw_count, holes, blocked);
            }

            int8_t shifted_x = x + i * dx;
            int8_t shifted_y = y + i * dy;

            if (shifted_x < 0 || shifted_x >= board.Width() || shifted_y < 0 ||
                shifted_y >= board.Height() ||
                *board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) !=
                    compute_for_stone_type ||
                *board.ChessPieceStateAt(MovePosition(shifted_x, shifted_y)) ==
                    StoneType::ST_NONE) {
                return std::make_tuple(raw_count, holes, blocked);
            } else {
                ++holes;
                state = LinkPatternDfaState::COUNT_PIECE;
            }
            break;
        }
        }
    }

    assert(false);
}

LinkStats LinkageAtWithDirection(int8_t x, int8_t y, int8_t dx, int8_t dy,
                                 StoneType compute_for_stone_type, GomokuBoardState const &board) {
    auto [raw_count1, holes1, blocked1] =
        LinkageCountsAtWithDirection(x, y, dx, dy, compute_for_stone_type, board);

    auto [raw_count2, holes2, blocked2] =
        LinkageCountsAtWithDirection(x, y, -dx, -dy, compute_for_stone_type, board);

    return LinkStats(raw_count1 + raw_count2, holes1 + holes2, blocked1 + blocked2);
}

std::array<LinkStats, 4> LinkagesAt(int8_t x, int8_t y, StoneType compute_for_stone_type,
                                    GomokuBoardState const &board) {
    LinkStats horizontal =
        LinkageAtWithDirection(x, y, /*dx=*/1, /*dy=*/0, compute_for_stone_type, board);

    LinkStats vertical =
        LinkageAtWithDirection(x, y, /*dx=*/0, /*dy =*/1, compute_for_stone_type, board);

    LinkStats diagnal =
        LinkageAtWithDirection(x, y, /*dx=*/1, /*dy=*/1, compute_for_stone_type, board);

    LinkStats counter_diagnal =
        LinkageAtWithDirection(x, y, /*dx=*/1, /*dy=*/-1, compute_for_stone_type, board);

    return std::array<LinkStats, 4>({horizontal, vertical, diagnal, counter_diagnal});
}

float LinkageToAdjustedCount(LinkStats const &linkage) {
    return linkage.raw_count - 0.5f * linkage.holes - linkage.blockage;
}

std::pair<float, float> ShlCount(int8_t x, int8_t y, StoneType compute_for_stone_type,
                                 GomokuBoardState const &board) {
    std::array<LinkStats, 4> linkages = LinkagesAt(x, y, compute_for_stone_type, board);

    std::array<float, 4> adjusted_counts;
    for (unsigned i = 0; i < 4; ++i) {
        adjusted_counts[i] = LinkageToAdjustedCount(linkages[i]);
    }

    std::sort(adjusted_counts.begin(), adjusted_counts.end(), std::greater<float>());

    float primary_shl_count = adjusted_counts[0];
    float secondary_shl_count = adjusted_counts[1];

    return std::make_pair(primary_shl_count, secondary_shl_count);
}

struct PositionAndShlScore {
    PositionAndShlScore(MovePosition const &position, ShlComponents const &shl_components);

    bool operator>(PositionAndShlScore const &rhs) const;

    MovePosition position;
    ShlComponents shl_components;
    float shl_score;
};

PositionAndShlScore::PositionAndShlScore(MovePosition const &position,
                                         ShlComponents const &shl_components)
    : position((position)), shl_components(shl_components),
      shl_score(shl_components.primary_shl_count_black + shl_components.secondary_shl_count_black +
                shl_components.primary_shl_count_white + shl_components.secondary_shl_count_white) {
}

bool PositionAndShlScore::operator>(PositionAndShlScore const &rhs) const {
    return shl_score > rhs.shl_score;
}

std::vector<std::pair<MovePosition, ShlComponents>>
NonTopKSuppression(std::vector<std::pair<MovePosition, ShlComponents>> const &raw_map, unsigned k,
                   float *shl_score_total) {
    std::vector<PositionAndShlScore> shl_scores;
    for (auto &[position, raw_score] : raw_map) {
        shl_scores.push_back(PositionAndShlScore(position, raw_score));
    }

    std::sort(shl_scores.begin(), shl_scores.end(), std::greater<PositionAndShlScore>());

    std::vector<std::pair<MovePosition, ShlComponents>> suppressed_map;
    *shl_score_total = 0.0f;
    for (unsigned i = 0; i < k && i < shl_scores.size(); ++i) {
        *shl_score_total += shl_scores[i].shl_score;
        suppressed_map.push_back(
            std::make_pair(shl_scores[i].position, shl_scores[i].shl_components));
    }

    return suppressed_map;
}

void Normalize(float shl_score_total,
               std::vector<std::pair<MovePosition, ShlComponents>> *shl_map) {
    if (shl_map->empty()) {
        return;
    }

    assert(shl_score_total > 0.0f);

    float norm_factor = 1.0f / shl_score_total;
    for (auto &[_, shl_components] : *shl_map) {
        shl_components.primary_shl_count_black *= norm_factor;
        shl_components.secondary_shl_count_black *= norm_factor;

        shl_components.primary_shl_count_white *= norm_factor;
        shl_components.secondary_shl_count_white *= norm_factor;
    }
}

} // namespace

LinkStats::LinkStats(int8_t raw_count, int8_t holes, bool blockage)
    : raw_count(raw_count), holes(holes), blockage(blockage) {}

ShlComponents::ShlComponents()
    : primary_shl_count_black(0.0f), secondary_shl_count_black(0.0f), primary_shl_count_white(0.0f),
      secondary_shl_count_white(0.0f) {}

ShlComponents::ShlComponents(float primary_shl_count_black, float secondary_shl_count_black,
                             float primary_shl_count_white, float secondary_shl_count_white)
    : primary_shl_count_black(primary_shl_count_black),
      secondary_shl_count_black(secondary_shl_count_black),
      primary_shl_count_white(primary_shl_count_white),
      secondary_shl_count_white(secondary_shl_count_white) {}

ShlFeatureBuilder::ShlFeatureBuilder(GomokuBoardState const &board)
    : width_(board.Width()), height_(board.Height()), double_contour_builder_(board, /*order=*/2) {
    for (auto pos : double_contour_builder_.Contour()) {
        this->UpdateShlFeaturesAt(pos, board);
    }
}

void ShlFeatureBuilder::UpdateShlFeaturesAt(MovePosition candid_pos,
                                            GomokuBoardState const &board) {
    auto [primary_black, secondary_black] =
        ShlCount(candid_pos.x, candid_pos.y, /*compute_for_stone_type=*/StoneType::ST_BLACK, board);
    auto [primary_white, secondary_white] =
        ShlCount(candid_pos.x, candid_pos.y, /*compute_for_stone_type=*/StoneType::ST_WHITE, board);

    if (primary_black == 0.0f && secondary_black == 0.0f && primary_white == 0.0f &&
        secondary_white == 0.0f) {
        raw_map_.erase(candid_pos);
        return;
    }

    auto it = raw_map_.find(candid_pos);
    if (it != raw_map_.end()) {
        it->second = ShlComponents(primary_black, secondary_black, primary_white, secondary_white);
    } else {
        raw_map_.insert(std::make_pair(candid_pos, ShlComponents(primary_black, secondary_black,
                                                                 primary_white, secondary_white)));
    }
}

void ShlFeatureBuilder::UpdateShlOverDirection(
    int dx, int dy, MovePosition pos, std::unordered_set<MovePosition> const &double_contour,
    GomokuBoardState const &board) {
    unsigned max_i = 6;

    if (dx < 0) {
        max_i = std::min(static_cast<unsigned>(-pos.x / dx), max_i);
    } else if (dx > 0) {
        max_i = std::min(static_cast<unsigned>((board.Width() - 1 - pos.x) / dx), max_i);
    }

    if (dy < 0) {
        max_i = std::min(static_cast<unsigned>(-pos.y / dy), max_i);
    } else if (dy > 0) {
        max_i = std::min(static_cast<unsigned>((board.Height() - 1 - pos.y) / dy), max_i);
    }

    for (unsigned i = 1; i <= max_i; ++i) {
        int8_t shifted_x = pos.x + i * dx;
        int8_t shifted_y = pos.y + i * dy;

        MovePosition candid_pos(shifted_x, shifted_y);

        if (*board.ChessPieceStateAt(candid_pos) != StoneType::ST_NONE ||
            double_contour.find(candid_pos) == double_contour.end()) {
            continue;
        }

        this->UpdateShlFeaturesAt(candid_pos, board);
    }
}

void ShlFeatureBuilder::AddStone(GomokuBoardState const &board) {
    assert(!board.History().empty());
    GomokuAction const &action = board.LastAction()->action.second;
    assert(action.stone_pos.has_value());

    double_contour_builder_.AddStone(*action.stone_pos);

    this->UpdateShlOverDirection(/*dx=*/-1, /*dy=*/0, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/-1, /*dy=*/-1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/0, /*dy=*/-1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/1, /*dy=*/-1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/1, /*dy=*/0, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/1, /*dy=*/1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/0, /*dy=*/1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);
    this->UpdateShlOverDirection(/*dx=*/-1, /*dy=*/1, *action.stone_pos,
                                 double_contour_builder_.Contour(), board);

    std::array<MovePosition, 8> new_potential_contour{
        MovePosition(action.stone_pos->x - 2, action.stone_pos->y + 1),
        MovePosition(action.stone_pos->x - 2, action.stone_pos->y - 1),
        MovePosition(action.stone_pos->x - 1, action.stone_pos->y - 2),
        MovePosition(action.stone_pos->x + 1, action.stone_pos->y - 2),
        MovePosition(action.stone_pos->x + 2, action.stone_pos->y - 1),
        MovePosition(action.stone_pos->x + 2, action.stone_pos->y + 1),
        MovePosition(action.stone_pos->x + 1, action.stone_pos->y + 2),
        MovePosition(action.stone_pos->x - 1, action.stone_pos->y + 2)};

    for (auto const &contour : new_potential_contour) {
        if (contour.x < 0 || contour.y < 0 || contour.x >= board.Width() ||
            contour.y >= board.Height() ||
            *board.ChessPieceStateAt(contour) != StoneType::ST_NONE ||
            raw_map_.find(contour) != raw_map_.end()) {
            continue;
        }

        this->UpdateShlFeaturesAt(contour, board);
    }

    raw_map_.erase(*action.stone_pos);
}

std::vector<std::pair<MovePosition, ShlComponents>>
ShlFeatureBuilder::TopKMapSparse(unsigned top_k, bool normalized,
                                 std::optional<StoneType> next_move_stone_type) const {
    std::vector<std::pair<MovePosition, ShlComponents>> adjusted_shl_map;

    for (auto const &[pos, shl_components] : raw_map_) {
        float primary_shl_count_black;
        float secondary_shl_count_black;
        float primary_shl_count_white;
        float secondary_shl_count_white;

        if (next_move_stone_type.has_value()) {
            switch (*next_move_stone_type) {
            case ST_BLACK: {
                primary_shl_count_black =
                    std::pow(std::max(0.0f, shl_components.primary_shl_count_black + 1.0f), 2.5f);
                secondary_shl_count_black =
                    std::pow(std::max(0.0f, shl_components.secondary_shl_count_black + 1.0f), 2.5f);
                primary_shl_count_white =
                    std::pow(std::max(0.0f, shl_components.primary_shl_count_white + 0.5f), 2.5f);
                secondary_shl_count_white =
                    std::pow(std::max(0.0f, shl_components.secondary_shl_count_white + 0.5f), 2.5f);
                break;
            }
            case ST_WHITE: {
                primary_shl_count_black =
                    std::pow(std::max(0.0f, shl_components.primary_shl_count_black + 0.5f), 2.5f);
                secondary_shl_count_black =
                    std::pow(std::max(0.0f, shl_components.secondary_shl_count_black + 0.5f), 2.5f);
                primary_shl_count_white =
                    std::pow(std::max(0.0f, shl_components.primary_shl_count_white + 1.0f), 2.5f);
                secondary_shl_count_white =
                    std::pow(std::max(0.0f, shl_components.secondary_shl_count_white + 1.0f), 2.5f);
                break;
            }
            default: {
                assert(false);
            }
            }
        } else {
            primary_shl_count_black =
                std::pow(std::max(0.0f, shl_components.primary_shl_count_black + 1.0f), 2.5f);
            secondary_shl_count_black =
                std::pow(std::max(0.0f, shl_components.secondary_shl_count_black + 1.0f), 2.5f);
            primary_shl_count_white =
                std::pow(std::max(0.0f, shl_components.primary_shl_count_white + 1.0f), 2.5f);
            secondary_shl_count_white =
                std::pow(std::max(0.0f, shl_components.secondary_shl_count_white + 1.0f), 2.5f);
        }

        adjusted_shl_map.push_back(
            std::make_pair(pos, ShlComponents(primary_shl_count_black, secondary_shl_count_black,
                                              primary_shl_count_white, secondary_shl_count_white)));
    }

    float score_total;
    std::vector<std::pair<MovePosition, ShlComponents>> suppressed_shl_map =
        NonTopKSuppression(adjusted_shl_map, top_k, &score_total);

    if (normalized) {
        Normalize(score_total, &suppressed_shl_map);
    }

    return suppressed_shl_map;
}

std::vector<float>
ShlFeatureBuilder::TopKMapDense(unsigned top_k, bool normalized,
                                std::optional<StoneType> next_move_stone_type) const {
    std::vector<std::pair<MovePosition, ShlComponents>> feature_map =
        this->TopKMapSparse(top_k, normalized, next_move_stone_type);

    std::vector<float> dense_map(width_ * height_ * 4);

    for (auto const &[pos, shl_components] : feature_map) {
        unsigned dst = (pos.x + pos.y * width_) * 4;
        dense_map[dst + 0] = shl_components.primary_shl_count_black;
        dense_map[dst + 1] = shl_components.secondary_shl_count_black;
        dense_map[dst + 2] = shl_components.primary_shl_count_white;
        dense_map[dst + 3] = shl_components.secondary_shl_count_white;
    }

    return dense_map;
}

std::vector<float> ShlFeatureBuilder::TopKShlPositionlessFeatures(
    unsigned top_k, bool normalized, std::optional<StoneType> next_move_stone_type) const {
    std::vector<std::pair<MovePosition, ShlComponents>> feature_map =
        this->TopKMapSparse(top_k, normalized, next_move_stone_type);

    std::vector<float> features(4 * top_k);

    for (unsigned i = 0; i < feature_map.size(); ++i) {
        auto const &[_, shl_components] = feature_map[i];
        features[i * 4 + 0] = shl_components.primary_shl_count_black;
        features[i * 4 + 1] = shl_components.secondary_shl_count_black;
        features[i * 4 + 2] = shl_components.primary_shl_count_white;
        features[i * 4 + 3] = shl_components.secondary_shl_count_black;
    }

    return features;
}

ShlFeatureBuilder &ShlFeatureBuilder::operator=(ShlFeatureBuilder const &rhs) {
    assert(width_ == rhs.width_);
    assert(height_ == rhs.height_);
    raw_map_ = rhs.raw_map_;
    double_contour_builder_ = rhs.double_contour_builder_;
    return *this;
}

float ToShlScore(e8::ShlComponents const &shl_components) {
    return shl_components.primary_shl_count_black + shl_components.secondary_shl_count_black +
           shl_components.primary_shl_count_white + shl_components.secondary_shl_count_white;
}

struct ShlFeatureBuilderCache::ShlFeatureBuilderCacheInternal {
    std::unordered_map<MctNodeId, ShlFeatureBuilder> cache_;
};

ShlFeatureBuilderCache::ShlFeatureBuilderCache()
    : pimpl_(std::make_unique<ShlFeatureBuilderCacheInternal>()) {}

ShlFeatureBuilderCache::~ShlFeatureBuilderCache() {}

ShlFeatureBuilder const &ShlFeatureBuilderCache::Update(std::optional<MctNodeId> parent_state_id,
                                                        MctNodeId state_id,
                                                        GomokuBoardState const &board) {
    auto current_state_it = pimpl_->cache_.find(state_id);
    if (current_state_it != pimpl_->cache_.end()) {
        auto const &[_, builder] = *current_state_it;
        return builder;
    }

    if (!parent_state_id.has_value()) {
        auto const &insertion =
            pimpl_->cache_.insert(std::make_pair(state_id, ShlFeatureBuilder(board)));
        assert(insertion.second == true);
        auto const &[_, builder] = *insertion.first;
        return builder;
    }

    auto parent_state_it = pimpl_->cache_.find(*parent_state_id);
    if (parent_state_it == pimpl_->cache_.end()) {
        assert(!board.History().empty());

        // Adds the missing parent.
        GomokuBoardState parent_board(board);
        parent_board.RetractAction();

        auto const &insertion = pimpl_->cache_.insert(
            std::make_pair(*parent_state_id, ShlFeatureBuilder(parent_board)));
        assert(insertion.second == true);

        parent_state_it = insertion.first;
    }

    auto const &insertion =
        pimpl_->cache_.insert(std::make_pair(state_id, ShlFeatureBuilder(parent_state_it->second)));
    assert(insertion.second);

    auto &[_, builder] = *insertion.first;

    auto action_record = board.LastAction();
    assert(action_record.has_value());
    GomokuAction const &action = action_record->action.second;
    if (action.stone_pos.has_value()) {
        builder.AddStone(board);
    }

    return builder;
}

void ShlFeatureBuilderCache::Clear() { pimpl_->cache_.clear(); }

} // namespace e8
