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

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/time_util/time_util.h"
#include "gomoku/agent/heuristics/tf_zero_prior_evaluator.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/agent_classroom/policy_iterator.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/logging/model_log_store.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

namespace e8 {
namespace {

unsigned const kCheckPointInterval = 100;
unsigned const kNumWarmUpGames = 1;

std::string ModelFileName(std::string const &model_path) {
    std::filesystem::path p(model_path);
    std::filesystem::directory_iterator end_it;

    std::vector<std::string> file_names;
    for (std::filesystem::directory_iterator it(p); it != end_it; ++it) {
        if (it->is_directory()) {
            std::string dir_name = it->path().filename().string();
            return dir_name;
        }
    }

    assert(false);
}

void GenerateEmptyModel(std::string const &source_tree_root, std::string const &model_output_path,
                        std::string const &model_name) {
    std::string generate_model_executable =
        source_tree_root + "/gomoku/agent_classroom/tfmodel/generate_model.py";
    int rc = std::system((generate_model_executable + " --model_output_path=" + model_output_path +
                          " --model_name=" + model_name)
                             .c_str());
    assert(rc == 0);
}

void TrainModel(std::string const &source_tree_root, std::string const &model_input_path,
                std::string const &model_output_path, GameLogPurpose data_source,
                std::string const &db_host_name, std::string const &db_name) {
    std::string train_model_executable =
        source_tree_root + "/gomoku/agent_classroom/tfmodel/train_model_brief.py";
    int rc = std::system((train_model_executable + " --model_input_path=" + model_input_path +
                          " --model_output_path=" + model_output_path +
                          " --data_source=" + std::to_string(data_source) +
                          " --db_host=" + db_host_name + " --db_name=" + db_name +
                          " --db_user=postgres --db_pass=password --num_data_entries=20000")
                             .c_str());
    assert(rc == 0);
}

GomokuModelEntity InitializeFirstModel(std::string const &model_name, bool bootstrap_from_rep_data,
                                       std::string const &source_tree_root,
                                       std::string const &model_storage_path,
                                       std::string const &db_host_name, std::string const &db_name,
                                       ModelLogStore *model_log_store) {
    TimestampMicros timestamp = CurrentTimestampMicros();

    std::string model_output_path = model_storage_path + "/" + std::to_string(timestamp);

    GenerateEmptyModel(source_tree_root, model_output_path, model_name);
    if (bootstrap_from_rep_data) {
        TrainModel(source_tree_root, model_output_path, model_output_path,
                   GameLogPurpose::GLP_REPRESENTATIVE_DATA, db_host_name, db_name);
    }

    std::string model_dir_name = ModelFileName(model_output_path);
    return model_log_store->LogNewModel(model_dir_name, model_output_path);
}

} // namespace

void IterateFromLastPolicy(GameInstanceContainer::ScheduleId schedule_id,
                           std::string const &model_name, unsigned num_iterations,
                           unsigned num_games_per_iteration, bool bootstrap_from_rep_data,
                           std::string const &source_tree_root,
                           std::string const &model_storage_path, std::string const &db_host_name,
                           std::string const &db_name, GameInstanceContainer *container) {
    ConnectionFactory conn_fact(ConnectionFactory::PQ, db_host_name, db_name);
    PooledConnectionReservoir conns(conn_fact);
    ModelLogStore model_log_store(&conns);

    // Generates learning material from the last created model.
    std::optional<GomokuModelEntity> last_model = model_log_store.LastModel();
    if (!last_model.has_value()) {
        last_model =
            InitializeFirstModel(model_name, bootstrap_from_rep_data, source_tree_root,
                                 model_storage_path, db_host_name, db_name, &model_log_store);
    }

    for (unsigned i = 0; i < num_iterations; ++i) {
        std::string model_dir_name = ModelFileName(*last_model->model_path.Value());
        auto evaluator = std::make_shared<GomokuTfZeroPriorEvaluator>(
            *last_model->model_path.Value() + "/" + model_dir_name);

        std::cout << "iteration=" << i << " games_played=" << i * num_games_per_iteration
                  << " model_name=" << model_dir_name << std::endl;

        GenerateLearningMaterial(GameLogPurpose::GLP_LEARNING_DATA, *last_model->id.Value(),
                                 evaluator, /*early_termination=*/false, schedule_id,
                                 num_games_per_iteration, db_host_name, db_name, container);

        if ((i + 1) * num_games_per_iteration < kNumWarmUpGames) {
            std::cout << "Skip training during warming up phase." << std::endl;
            continue;
        }

        if (i % kCheckPointInterval == 0) {
            // Creates a new model by training the last model on the newly generated learning
            // materials.
            TimestampMicros timestamp = CurrentTimestampMicros();
            std::string new_model_path = model_storage_path + "/" + std::to_string(timestamp);

            TrainModel(source_tree_root, *last_model->model_path.Value(), new_model_path,
                       GameLogPurpose::GLP_LEARNING_DATA, db_host_name, db_name);

            model_dir_name = ModelFileName(new_model_path);
            GomokuModelEntity new_model =
                model_log_store.LogNewModel(model_dir_name, new_model_path);

            last_model = new_model;
        } else {
            // Simply updates the latest model.
            TrainModel(source_tree_root, *last_model->model_path.Value(),
                       *last_model->model_path.Value(), GameLogPurpose::GLP_LEARNING_DATA,
                       db_host_name, db_name);
        }
    }
}

} // namespace e8
