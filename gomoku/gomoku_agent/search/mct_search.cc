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
#include <iostream>
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

float UpperConfidenceBound(float const exploration_factor, MctNode const &parent,
                           MctNode const &node) {
    float q_value = node.summed_reward / (1.0f + node.num_bandit_pulls);
    float uncertainty = exploration_factor * std::sqrt(1.0f + parent.num_bandit_pulls) /
                        (1.0f + node.num_bandit_pulls);

    assert(q_value < 1.05f && q_value > -1.05f);
    assert(!std::isinf(q_value));
    assert(!std::isnan(q_value));
    assert(!std::isinf(uncertainty));
    assert(!std::isnan(uncertainty));

    return q_value + node.heuristic_policy_weight * uncertainty;
}

void UpdateMctNode(EvaluationResult const &eval, float const exploration_factor, MctNode *parent,
                   MctNode *node) {
    assert(node->action_performed_by.has_value());
    node->summed_reward += eval.reward_viewed_by_player[*node->action_performed_by];
    node->upper_confidence_bound = UpperConfidenceBound(exploration_factor, *parent, *node);
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

EvaluationResult Evaluate(GomokuBoardState const &state, MctNode const &state_mct_node,
                          GomokuEvaluatorInterface *evaluator) {

    GameResult game_result = state.CurrentGameResult();

    EvaluationResult result;

    switch (game_result) {
    case GR_PLAYER_A_WIN: {
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_A] = 1.0f;
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_B] = -1.0f;
        break;
    }
    case GR_PLAYER_B_WIN: {
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_A] = -1.0f;
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_B] = 1.0f;
        break;
    }
    case GR_TIE: {
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_A] = 0.0f;
        result.reward_viewed_by_player[PlayerSide::PS_PLAYER_B] = 0.0f;
        break;
    }
    case GR_UNDETERMINED: {
        float est_reward =
            evaluator->EvaluateReward(state, reinterpret_cast<GomokuStateId>(&state_mct_node));

        assert(est_reward < 1.05f && est_reward > -1.05f);
        assert(!std::isinf(est_reward));
        assert(!std::isnan(est_reward));

        result.reward_viewed_by_player[state.CurrentPlayerSide()] = est_reward;
        result.reward_viewed_by_player[(state.CurrentPlayerSide() + 1) & 1] = -est_reward;
        break;
    }
    }

    return result;
}

void Expand(MctNode *root, GomokuBoardState *state, GomokuEvaluatorInterface *evaluator) {
    std::unordered_map<GomokuActionId, float> heuristics_policy =
        evaluator->EvaluatePolicy(*state, reinterpret_cast<GomokuStateId>(root));
    PlayerSide const action_performer = state->CurrentPlayerSide();

    std::unordered_map<GomokuActionId, GomokuAction> actions = state->LegalActions();
    for (auto const &[action_id, _] : actions) {
        GameResult game_result = state->ApplyAction(action_id,
                                                    /*cached_game_result=*/std::nullopt);
        state->RetractAction();

        auto policy_weight_it = heuristics_policy.find(action_id);
        assert(policy_weight_it != heuristics_policy.end());

        MctNode child(action_id, action_performer, game_result, policy_weight_it->second);
        child.upper_confidence_bound =
            UpperConfidenceBound(evaluator->ExplorationFactor(), *root, child);
        root->children.push(child);
    }
}

void SelectFrom(MctNode *root, GomokuBoardState *state, GomokuEvaluatorInterface *evaluator,
                std::vector<MutablePriorityQueue<MctNode>::iterator> *propagation_path) {
    if (state->CurrentGameResult() != GR_UNDETERMINED) {
        EvaluationResult eval = Evaluate(*state, *root, evaluator);
        BackPropagate(eval, evaluator->ExplorationFactor(), propagation_path);
        return;
    }

    if (root->children.empty()) {
        Expand(root, state, evaluator);
        EvaluationResult eval = Evaluate(*state, *root, evaluator);
        BackPropagate(eval, evaluator->ExplorationFactor(), propagation_path);
        return;
    }

    auto candidate_it = root->children.front();
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

void PrintMctsStats(MctNode const &root, GomokuBoardState const &state) {
    std::cout << "--------------------------------" << std::endl;

    for (auto const &child : root.children) {
        if (child.num_bandit_pulls > 0) {
            assert(child.arrived_thru_action_id.has_value());
            GomokuAction const &action = state.LegalActions().at(*child.arrived_thru_action_id);

            if (action.stone_pos.has_value()) {
                std::cout << "(" << static_cast<int>(action.stone_pos->x) << ","
                          << static_cast<int>(action.stone_pos->y) << ")";
            }

            if (action.swap2_decision.has_value()) {
                switch (*action.swap2_decision) {
                case SW2D_CHOOSE_BLACK: {
                    std::cout << "SW2D_CHOOSE_BLACK";
                    break;
                }
                case SW2D_CHOOSE_WHITE: {
                    std::cout << "SW2D_CHOOSE_WHITE";
                    break;
                }
                case SW2D_PLACE_2_STONES: {
                    std::cout << "SW2D_PLACE_2_STONES";
                    break;
                }
                }
            }

            if (*action.stone_type_decision) {
                switch (*action.stone_type_decision) {
                case STD_CHOOSE_BLACK: {
                    std::cout << "STD_CHOOSE_BLACK";
                    break;
                }
                case STD_CHOOSE_WHITE: {
                    std::cout << "STD_CHOOSE_WHITE";
                    break;
                }
                }
            }

            std::cout << "|reward=" << child.summed_reward / child.num_bandit_pulls
                      << "|n=" << child.num_bandit_pulls << "|ucb=" << child.upper_confidence_bound
                      << "|p=" << child.heuristic_policy_weight << std::endl;
        }
    }

    std::cout << "--------------------------------" << std::endl;
}

} // namespace

std::unordered_map<GomokuActionId, float>
MctSearchFrom(GomokuBoardState state, GomokuEvaluatorInterface *evaluator, bool const print_stats) {
    evaluator->ClearCache();

    MutablePriorityQueue<MctNode> root;
    root.push(MctNode(
        /*arrived_thru_action_id=*/std::nullopt, /*action_performed_by=*/std::nullopt,
        state.CurrentGameResult(),
        /*heuristic_policy_weight=*/std::numeric_limits<float>::infinity()));

    std::vector<MutablePriorityQueue<MctNode>::iterator> propagation_path{root.begin()};
    for (unsigned i = 0; i < evaluator->NumSimulations(); ++i) {
        SelectFrom(&(*root.begin()), &state, evaluator, &propagation_path);
    }

    if (print_stats) {
        PrintMctsStats(*root.begin(), state);
    }

    return ExtractStochasticPolicy(*root.begin());
}

GomokuActionId BestAction(std::unordered_map<GomokuActionId, float> const &optimal_policy) {
    assert(!optimal_policy.empty());
    GomokuActionId best_action = -1;
    float best_p = -1.0f;
    for (auto const &[action_id, p] : optimal_policy) {
        if (p > best_p) {
            best_action = action_id;
            best_p = p;
        }
    }
    return best_action;
}

} // namespace e8
