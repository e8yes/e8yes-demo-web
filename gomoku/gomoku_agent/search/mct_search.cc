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
#include <limits>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "common/container/mutable_priority_queue.h"
#include "gomoku/gomoku_agent/heuristics/evaluator.h"
#include "gomoku/gomoku_agent/search/mct_node.h"
#include "gomoku/gomoku_agent/search/mct_search.h"
#include "gomoku/gomoku_game/board_state.h"

namespace e8 {
namespace {

struct EvaluationResult {
    std::array<float, 2> reward_viewed_by_player;
};

void UpdateMctNode(EvaluationResult const &eval, float const exploration_factor, MctNode *parent,
                   MctNode *node) {
    assert(node->action_performed_by.has_value());
    node->summed_reward += eval.reward_viewed_by_player[*node->action_performed_by];
    node->upper_confidence_bound = node->summed_reward / node->num_bandit_pulls +
                                   exploration_factor * node->heuristic_policy_weight *
                                       std::sqrt(parent->num_bandit_pulls) /
                                       (1 + node->num_bandit_pulls);
    node->num_bandit_pulls += 1;
}

void BackPropagate(EvaluationResult const &eval, float const exploration_factor,
                   std::vector<MutablePriorityQueue<MctNode>::iterator> *propagation_path) {
    for (int i = propagation_path->size() - 1; i >= 1; --i) {
        MctNode *parent = &(*(*propagation_path)[i - 1]);
        MctNode *node = &(*(*propagation_path)[i]);
        UpdateMctNode(eval, exploration_factor, parent, node);

        parent->children.reprioritize((*propagation_path)[i]);
    }
    (*propagation_path)[0]->num_bandit_pulls += 1;
}

EvaluationResult Evaluate(GomokuBoardState const &state, GomokuEvaluatorInterface const &evaluator,
                          MctNode const &state_mct_node) {

    GameResult game_result = state.CurrentGameResult();
    assert(state_mct_node.action_performed_by.has_value());

    EvaluationResult result;

    switch (game_result) {
    case GR_PLAYER_A_WIN:
    case GR_PLAYER_B_WIN: {
        result.reward_viewed_by_player[*state_mct_node.action_performed_by] = 1.0f;
        result.reward_viewed_by_player[(*state_mct_node.action_performed_by + 1) | 1] = -1.0f;
        break;
    }
    case GR_TIE: {
        result.reward_viewed_by_player[*state_mct_node.action_performed_by] = 0.0f;
        result.reward_viewed_by_player[(*state_mct_node.action_performed_by + 1) | 1] = 0.0f;
        break;
    }
    case GR_UNDETERMINED: {
        float est_reward =
            evaluator.EvaluateReward(state, reinterpret_cast<GomokuStateId>(&state_mct_node));
        result.reward_viewed_by_player[*state_mct_node.action_performed_by] = est_reward;
        result.reward_viewed_by_player[(*state_mct_node.action_performed_by + 1) | 1] = -est_reward;
        break;
    }
    }

    return result;
}

void Expand(MctNode *root, GomokuBoardState *state, GomokuEvaluatorInterface const &evaluator) {
    std::unordered_map<GomokuActionId, float> heuristics_policy =
        evaluator.EvaluatePolicy(*state, reinterpret_cast<GomokuStateId>(root));
    PlayerSide const action_performer = state->CurrentPlayerSide();

    for (auto const &[action_id, _] : state->LegalActions()) {
        GameResult game_result = state->ApplyAction(action_id,
                                                    /*cached_game_result=*/std::nullopt);
        state->RetractAction();

        auto policy_weight_it = heuristics_policy.find(action_id);
        assert(policy_weight_it != heuristics_policy.end());

        root->children.push(
            MctNode(action_id, action_performer, game_result, policy_weight_it->second));
    }
}

void SelectFrom(MctNode *root, GomokuBoardState *state, GomokuEvaluatorInterface const &evaluator,
                std::vector<MutablePriorityQueue<MctNode>::iterator> *propagation_path) {
    if (state->CurrentGameResult() != GR_UNDETERMINED) {
        EvaluationResult eval = Evaluate(*state, evaluator, *root);
        BackPropagate(eval, evaluator.ExplorationFactor(), propagation_path);
        return;
    }

    if (root->children.empty()) {
        Expand(root, state, evaluator);
        EvaluationResult eval = Evaluate(*state, evaluator, *root);
        BackPropagate(eval, evaluator.ExplorationFactor(), propagation_path);
        return;
    }

    auto candidate_it = root->children.begin();
    state->ApplyAction(*candidate_it->arrived_thru_action_id, candidate_it->game_result);
    propagation_path->push_back(candidate_it);

    SelectFrom(&(*candidate_it), state, evaluator, propagation_path);

    propagation_path->pop_back();
    state->RetractAction();
}

std::unordered_map<GomokuActionId, float> ExtractStochasticPolicy(MctNode const &root) {
    std::unordered_map<GomokuActionId, float> policy(root.children.size());
    unsigned total_num_bandit_pulls = 0;
    for (auto const &child : root.children) {
        assert(child.arrived_thru_action_id.has_value());
        policy[*child.arrived_thru_action_id] = child.num_bandit_pulls;
        total_num_bandit_pulls += child.num_bandit_pulls;
    }

    assert(total_num_bandit_pulls > 0);

    for (auto &[_, p] : policy) {
        p /= total_num_bandit_pulls;
    }

    return policy;
}

} // namespace

std::unordered_map<GomokuActionId, float> MctSearchFrom(GomokuBoardState state,
                                                        GomokuEvaluatorInterface const &evaluator) {
    MutablePriorityQueue<MctNode> root;
    root.push(MctNode(
        /*arrived_thru_action_id=*/std::nullopt, /*action_performed_by=*/std::nullopt,
        state.CurrentGameResult(),
        /*heuristic_policy_weight=*/std::numeric_limits<float>::infinity()));

    std::vector<MutablePriorityQueue<MctNode>::iterator> propagation_path{root.begin()};

    for (unsigned i = 0; i < evaluator.NumSimulations(); ++i) {
        SelectFrom(&(*root.begin()), &state, evaluator, &propagation_path);
    }

    return ExtractStochasticPolicy(*root.begin());
}

} // namespace e8
