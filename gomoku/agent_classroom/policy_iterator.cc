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
#include <memory>
#include <optional>
#include <regex>
#include <string>
#include <vector>

#include "common/time_util/time_util.h"
#include "gomoku/agent/heuristics/model_based_evaluator.h"
#include "gomoku/agent_classroom/learning_material_generator.h"
#include "gomoku/agent_classroom/policy_iterator.h"
#include "gomoku/game/game_instance_container.h"
#include "gomoku/logging/model_log_store.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

namespace e8 {
namespace {

std::vector<std::string> TfLiteModelFileName(std::string const &model_path) {
    std::filesystem::path p(model_path);
    std::filesystem::directory_iterator end_it;

    std::regex tflite_regex("(.+).tflite");
    std::vector<std::string> tflite_file_names;
    for (std::filesystem::directory_iterator it(p); it != end_it; ++it) {
        std::smatch sm;
        if (it->is_regular_file()) {
            std::string file_name = it->path().filename().string();
            if (!std::regex_match(file_name, sm, tflite_regex)) {
                continue;
            }

            assert(sm.size() == 2);
            tflite_file_names.push_back(sm[1].str());
        }
    }

    return tflite_file_names;
}

void GenerateEmptyModel(std::string const &source_tree_root, std::string const &model_output_path) {
    std::string generate_model_executable =
        source_tree_root + "/gomoku/agent_classroom/tfmodel/generate_model.py";
    int rc = std::system(
        (generate_model_executable + " --model_output_path=" + model_output_path).c_str());
    assert(rc == 0);
}

void TrainModel(std::string const &source_tree_root, std::string const &model_input_path,
                std::string const &model_output_path, GameLogPurpose data_source,
                std::string const &db_host_name, std::string const &db_name) {
    std::string train_model_executable =
        source_tree_root + "/gomoku/agent_classroom/tfmodel/train_model.py";
    int rc = std::system((train_model_executable + " --model_input_path=" + model_input_path +
                          " --model_output_path=" + model_output_path +
                          " --data_source=" + std::to_string(data_source) +
                          " --db_host=" + db_host_name + " --db_name=" + db_name +
                          " --db_user=postgres --db_pass=password --num_data_entries=40000")
                             .c_str());
    assert(rc == 0);
}

void ConvertToLiteModel(std::string const &source_tree_root, std::string const &model_input_path) {
    std::string converter_executable =
        source_tree_root + "/gomoku/agent_classroom/tfmodel/tflite_converter.py";
    int rc =
        std::system((converter_executable + " --model_input_path=" + model_input_path).c_str());
    assert(rc == 0);
}

GomokuModelEntity InitializeFirstModel(bool bootstrap_from_rep_data,
                                       std::string const &source_tree_root,
                                       std::string const &model_storage_path,
                                       std::string const &db_host_name, std::string const &db_name,
                                       ModelLogStore *model_log_store) {
    TimestampMicros timestamp = CurrentTimestampMicros();

    std::string model_output_path = model_storage_path + "/" + std::to_string(timestamp);

    GenerateEmptyModel(source_tree_root, model_output_path);
    if (bootstrap_from_rep_data) {
        TrainModel(source_tree_root, model_output_path, model_output_path,
                   GameLogPurpose::GLP_REPRESENTATIVE_DATA, db_host_name, db_name);
    }
    ConvertToLiteModel(source_tree_root, model_output_path);

    std::vector<std::string> model_names = TfLiteModelFileName(model_output_path);
    assert(model_names.size() == 1);
    return model_log_store->LogNewModel(model_names[0], model_output_path);
}

} // namespace

void IterateFromLastPolicy(GameInstanceContainer::ScheduleId schedule_id, unsigned num_iterations,
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
            InitializeFirstModel(bootstrap_from_rep_data, source_tree_root, model_storage_path,
                                 db_host_name, db_name, &model_log_store);
    }

    for (unsigned i = 0; i < num_iterations; ++i) {
        std::vector<std::string> model_names = TfLiteModelFileName(*last_model->model_path.Value());
        assert(model_names.size() == 1);
        auto evaluator = std::make_shared<GomokuModelBasedEvaluator>(
            *last_model->model_path.Value() + "/" + model_names[0] + ".tflite");

        GenerateLearningMaterial(GameLogPurpose::GLP_LEARNING_DATA, *last_model->id.Value(),
                                 evaluator, schedule_id, num_games_per_iteration, db_host_name,
                                 db_name, container);

        // Creates a new model by training the last model on the newly generated learning materials.
        TimestampMicros timestamp = CurrentTimestampMicros();
        std::string new_model_path = model_storage_path + "/" + std::to_string(timestamp);

        TrainModel(source_tree_root, *last_model->model_path.Value(), new_model_path,
                   GameLogPurpose::GLP_LEARNING_DATA, db_host_name, db_name);
        ConvertToLiteModel(source_tree_root, new_model_path);

        GomokuModelEntity new_model =
            model_log_store.LogNewModel(evaluator->ModelName(), new_model_path);

        last_model = new_model;
    }
}

} // namespace e8
