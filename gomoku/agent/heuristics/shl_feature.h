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

#ifndef SHL_FEATURE_H
#define SHL_FEATURE_H

#include <cstdint>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gomoku/agent/heuristics/contour.h"
#include "gomoku/game/board_state.h"

namespace e8 {

/**
 * @brief The LinkStats struct Statistics for a link viewed at certain position. Each position
 * involves exactly four potential links laid in horizontal, vertical, diagnal and counter diagnal
 * direction centered at the viewer position. A link: cannot have any consecutive holes; within a
 * 5-cell radius from the viewer position; consists of only one type of stones.
 */
struct LinkStats {
    LinkStats(int8_t raw_count, int8_t holes, bool blockage);

    // The number of stones in a link.
    int8_t raw_count;

    // The number of holes in a link.
    int8_t holes;

    // How many blockage is there in a link. Each link can have 0 to 2 blockages (open, half-closed
    // and fully closed).
    bool blockage;
};

/**
 * @brief The ShlComponents struct Score features for the primary and secondary links for each stone
 * type.
 */
struct ShlComponents {
    ShlComponents();
    ShlComponents(float primary_shl_count_black, float secondary_shl_count_black,
                  float primary_shl_count_white, float secondary_shl_count_white);

    float primary_shl_count_black;
    float secondary_shl_count_black;
    float primary_shl_count_white;
    float secondary_shl_count_white;
};

/**
 * @brief The ShlFeatureBuilder class It helps extract the Suggestive Hotspot Links (SHL) features
 * from the Gomoku board and provies data strctures to allow effcient incremental update. The
 * SHL features are extracted by looking at all possible links from all the double contour
 * positions for both stone types (See LinkStats for how a link is defined). An adjusted count is
 * then assigned for each link and stone type and is caluated as follow:
 *
 * adjusted_count = |raw_count + move_advantage - 0.5*holes - blockage|^2.5. Move advantage is set
 * to 1 if the next_move_stone_type is the same as the adjusted count's stone type. Otherwise, it is
 * set to 0.5. If the next_move_stone_type is not supplied, move_advantage is default to 1.
 *
 * For each stone typep, the top 2 links are selected at each position based on this adjusted count
 * and are called the primary and secondary counts, respectively. These counts constitutes the
 * raw_map.
 *
 * Then it computes the SHL score using those adjusted counts for each position:
 *
 * shl_score = primary_count_black + secondary_count_black + primary_count_white +
 * secondary_count_white
 *
 * Eventually, it ranks each position's adjusted count by the shl_score and selects the top K
 * positions. Other positions are erased. With the unsuppressed positions, the adjusted counts are
 * normalized by the sum of the remaining SHL scores. This gives the final SHL map.
 */
class ShlFeatureBuilder {
  public:
    /**
     * @brief ShlFeatureBuilder Builds a brand new SHL feature map for the specified board state.
     */
    ShlFeatureBuilder(GomokuBoardState const &board);

    /**
     * @brief AddStone Synchronizes the SHL feature map with board after a stone is placed  onto the
     * board. It detects the affected SHL map positions and updates their feature values.
     *
     * @param board The board must be one stone different than the feature map. The last action in
     * the board must be a stone action.
     */
    void AddStone(GomokuBoardState const &board);

    /**
     * @brief NormalizedTopKMapSparse Processes the SHL features to a sparse 2D map storing the
     * non-top-k-suppressed and normalized SHL components. The ordering of the following list
     * indicates the rank of those top K ShlComponents. This list may not get filled up to exactly K
     * elements in the case when there isn't K SHL candidate scores.
     *
     * @param top_k Non-top-K-suppression parameter.
     * @param normalized Whether to normalize the remaining scores after supression.
     * @return Ranked SHL features.
     */
    std::vector<std::pair<MovePosition, ShlComponents>>
    TopKMapSparse(unsigned top_k, bool normalized,
                  std::optional<StoneType> next_move_stone_type) const;

    /**
     * @brief NormalizedTopKMapDense The original SHL map produced by TopKMapSparse() is stored in
     * sparse format. This function converts the map to a dense flat array in row major data layout.
     *
     * @param top_k Non-top-K-suppression parameter.
     * @param normalized Whether to normalize the remaining scores after supression.
     * @return A row major flat array representing the dense 3D ShL map with shape (width, height,
     * 4).
     */
    std::vector<float> TopKMapDense(unsigned top_k, bool normalized,
                                    std::optional<StoneType> next_move_stone_type) const;

    /**
     * @brief TopKShlPositionlessFeatures Takes the top K SHL features then flattens them and strips
     * away position information.
     *
     * @param top_k Non-top-K-suppression parameter.
     */
    std::vector<float>
    TopKShlPositionlessFeatures(unsigned top_k, bool normalized,
                                std::optional<StoneType> next_move_stone_type) const;

  private:
    void UpdateShlFeaturesAt(MovePosition candid_pos, GomokuBoardState const &board);

    void UpdateShlOverDirection(int dx, int dy, MovePosition pos,
                                std::unordered_set<MovePosition> const &double_contour,
                                GomokuBoardState const &board);

    // Width and height of the feature map. Should be in the same size of the board.
    unsigned const width_;
    unsigned const height_;

    // Sparse 2D map storing the raw SHL components.
    std::unordered_map<MovePosition, ShlComponents> raw_map_;

    ContourBuilder double_contour_builder_;
};

/**
 * @brief ToShlScore Computes the SHL score from the SHL components.
 *
 * shl_score = primary_count_black + secondary_count_black + primary_count_white +
 * secondary_count_white
 */
float ToShlScore(e8::ShlComponents const &shl_components);

} // namespace e8

#endif // SHL_FEATURE_H
