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
#include <cmath>
#include <cstdint>
#include <optional>
#include <tuple>
#include <utility>
#include <vector>

#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/agent/heuristics/shl_feature.h"
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

float LinkageToAdjustedCount(LinkStats const &linkage, StoneType compute_for_stone_type,
                             std::optional<StoneType> next_move_stone_type) {
    float move_advantage;
    if (next_move_stone_type.has_value()) {
        if (compute_for_stone_type == *next_move_stone_type) {
            move_advantage = 1;
        } else {
            move_advantage = 0.5f;
        }
    } else {
        move_advantage = 1.0f;
    }

    float adjusted_count = std::max(0.0f, linkage.raw_count + move_advantage -
                                              0.5f * linkage.holes - linkage.blockage);

    return adjusted_count;
}

std::pair<float, float> ShlCount(int8_t x, int8_t y, StoneType compute_for_stone_type,
                                 std::optional<StoneType> next_move_stone_type,
                                 GomokuBoardState const &board) {
    std::array<LinkStats, 4> linkages = LinkagesAt(x, y, compute_for_stone_type, board);

    std::array<float, 4> adjusted_counts;
    for (unsigned i = 0; i < 4; ++i) {
        adjusted_counts[i] =
            LinkageToAdjustedCount(linkages[i], compute_for_stone_type, next_move_stone_type);
    }

    std::sort(adjusted_counts.begin(), adjusted_counts.end(), std::greater<float>());

    float primary_shl_count = std::pow(adjusted_counts[0], 2.5f);
    float secondary_shl_count = std::pow(adjusted_counts[1], 2.5f);

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

void NonTopKSuppression(unsigned k, ShlFeatures *shl_map) {
    std::vector<PositionAndShlScore> shl_scores;
    for (auto &[position, raw_score] : shl_map->raw_map) {
        shl_scores.push_back(PositionAndShlScore(position, raw_score));
    }

    std::sort(shl_scores.begin(), shl_scores.end(), std::greater<PositionAndShlScore>());

    shl_map->shl_score_total = 0.0f;
    for (unsigned i = 0; i < k && i < shl_scores.size(); ++i) {
        shl_map->shl_score_total += shl_scores[i].shl_score;
        shl_map->normalized_top_k_map.push_back(
            std::make_pair(shl_scores[i].position, shl_scores[i].shl_components));
    }
}

void Normalize(ShlFeatures *shl_map) {
    if (shl_map->raw_map.empty()) {
        return;
    }

    assert(shl_map->shl_score_total > 0.0f);

    float norm_factor = 1.0f / shl_map->shl_score_total;
    for (auto &[_, shl_components] : shl_map->normalized_top_k_map) {
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

ShlFeatures::ShlFeatures(unsigned width, unsigned height, unsigned top_k)
    : width(width), height(height), top_k(top_k) {}

ShlFeatures ComputeShlFeatures(GomokuBoardState const &board,
                               std::unordered_set<MovePosition> const &double_contour,
                               std::optional<StoneType> next_move_stone_type, unsigned top_k) {
    assert(top_k >= 1);

    ShlFeatures shl_map(board.Width(), board.Height(), top_k);

    for (auto candid_pos : double_contour) {
        auto [primary_black, secondary_black] =
            ShlCount(candid_pos.x, candid_pos.y, /*compute_for_stone_type=*/StoneType::ST_BLACK,
                     next_move_stone_type, board);
        auto [primary_white, secondary_white] =
            ShlCount(candid_pos.x, candid_pos.y, /*compute_for_stone_type=*/StoneType::ST_WHITE,
                     next_move_stone_type, board);

        shl_map.raw_map.push_back(
            std::make_pair(candid_pos, ShlComponents(primary_black, secondary_black, primary_white,
                                                     secondary_white)));
    }

    NonTopKSuppression(top_k, &shl_map);
    Normalize(&shl_map);

    return shl_map;
}

std::vector<float> ToDenseShlMap(ShlFeatures const &feature_map) {
    std::vector<float> dense_map(feature_map.width * feature_map.height * 4);

    for (auto const &[pos, shl_components] : feature_map.normalized_top_k_map) {
        unsigned dst = (pos.x + pos.y * feature_map.width) * 4;
        dense_map[dst + 0] = shl_components.primary_shl_count_black;
        dense_map[dst + 1] = shl_components.secondary_shl_count_black;
        dense_map[dst + 2] = shl_components.primary_shl_count_white;
        dense_map[dst + 3] = shl_components.secondary_shl_count_white;
    }

    return dense_map;
}

std::vector<float> TopKShlPositionlessFeatures(ShlFeatures const &feature_map) {
    std::vector<float> features(4 * feature_map.top_k);

    for (unsigned i = 0; i < feature_map.normalized_top_k_map.size(); ++i) {
        auto const &[_, shl_components] = feature_map.normalized_top_k_map[i];
        features[i * 4 + 0] = shl_components.primary_shl_count_black;
        features[i * 4 + 1] = shl_components.secondary_shl_count_black;
        features[i * 4 + 2] = shl_components.primary_shl_count_white;
        features[i * 4 + 3] = shl_components.secondary_shl_count_black;
    }

    return features;
}

float ToShlScore(e8::ShlComponents const &shl_components) {
    return shl_components.primary_shl_count_black + shl_components.secondary_shl_count_black +
           shl_components.primary_shl_count_white + shl_components.secondary_shl_count_white;
}

} // namespace e8
