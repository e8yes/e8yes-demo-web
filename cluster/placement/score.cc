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

#include <vector>

#include "cluster/placement/capability.h"
#include "cluster/placement/common_types.h"
#include "cluster/placement/score.h"
#include "proto_cc/machine.pb.h"

namespace e8 {
namespace {

WeightedCapabilities::Type MostDemandingCapability(WeightedCapabilities const &capabilities) {
    WeightedCapabilities::FixedPoint most_demanding_capability_value =
        CapabilityFixedPointFromFloat(0.0f);
    WeightedCapabilities::Type most_demanding_capability = WeightedCapabilities::INVALID_TYPE;

    for (auto type : WeightedCapabilityTypes()) {
        WeightedCapabilities::FixedPoint value = GetCapabilityByType(type, capabilities);
        if (value > most_demanding_capability_value) {
            most_demanding_capability = type;
            most_demanding_capability_value = value;
        }
    }

    return most_demanding_capability;
}

} // namespace

CapabilityScoreInterface::CapabilityScoreInterface() {}

CapabilityScoreInterface::~CapabilityScoreInterface() {}

std::vector<float>
CapabilityScoreInterface::Score(std::vector<WeightedCapabilities> const &candidates,
                                ResourceDescriptor const & /*resource*/) const {
    std::vector<float> uniform_score(candidates.size());

    float score = 1.0f / candidates.size();
    for (unsigned i = 0; i < candidates.size(); ++i) {
        uniform_score[i] = score;
    }

    return uniform_score;
}

MostDemandingCapabilityScore::MostDemandingCapabilityScore() {}

MostDemandingCapabilityScore::~MostDemandingCapabilityScore() {}

std::vector<float>
MostDemandingCapabilityScore::Score(std::vector<WeightedCapabilities> const &candidates,
                                    ResourceDescriptor const &resource) const {
    WeightedCapabilities::Type most_demanding_capability =
        MostDemandingCapability(resource.required_capabilities);
    if (most_demanding_capability == e8::WeightedCapabilities::INVALID_TYPE) {
        // A don't-care resource.
        return CapabilityScoreInterface::Score(candidates, resource);
    }

    std::vector<float> score(candidates.size());
    float normalizer = 0.0f;

    for (unsigned i = 0; i < candidates.size(); ++i) {
        float candid_value = ToFloat(GetCapabilityByType(most_demanding_capability, candidates[i]));

        score[i] = candid_value;
        normalizer += candid_value;
    }

    if (normalizer == 0.0f) {
        // None of the candidates is able to satisfy the resource.
        return std::vector<float>();
    }

    for (unsigned i = 0; i < candidates.size(); ++i) {
        score[i] /= normalizer;
    }

    return score;
}

} // namespace e8
