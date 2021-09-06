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

#ifndef PLACEMENT_SCORE_H
#define PLACEMENT_SCORE_H

#include <vector>

#include "cluster/placement/common_types.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief The CapabilityScoreInterface class The interface to score candidate nodes against a
 * resource. This base class always returns a uniform score.
 */
class CapabilityScoreInterface {
  public:
    CapabilityScoreInterface();
    virtual ~CapabilityScoreInterface();

    /**
     * @brief Score Scores each candidate's capability value in according to the resource. The
     * scores are normalized so that the sum of the candidate scores is 1. If there isn't any
     * suitable candidate, it returns an empty list.
     *
     * @param candidates The list of candidates to be scored.
     * @param resource The resource to be scored against.
     * @return The score of each candidate, if possible. Otherwise, it returns an empty list.
     */
    virtual std::vector<float> Score(std::vector<WeightedCapabilities> const &candidates,
                                     ResourceDescriptor const &resource) const;
};

/**
 * @brief The MostDemandingCapabilityScore class It scores the candidates based on the resource's
 * most demanding capability (TODO: standardize the capability values). If the resource doesn't have
 * any specified required capabilities, it will return a uniform score.
 */
class MostDemandingCapabilityScore : public CapabilityScoreInterface {
  public:
    MostDemandingCapabilityScore();
    ~MostDemandingCapabilityScore();

    std::vector<float> Score(std::vector<WeightedCapabilities> const &candidates,
                             ResourceDescriptor const &resource) const override;
};

} // namespace e8

#endif // PLACEMENT_SCORER_H
