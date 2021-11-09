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

#include <chrono>
#include <cstdio>
#include <google/protobuf/repeated_field.h>
#include <optional>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cluster/task_manager/history.h"
#include "cluster/task_manager/registry.h"
#include "cluster/task_manager/startup.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/task.pb.h"

namespace {

static constexpr char const *kHistoryPeristencePath = "./test_task_history";
static constexpr char const *kStartupListPeristencePath = "./test_startup_list";

struct TestData {
    TestData();
    ~TestData();

    e8::TaskHistoryStore history;
    e8::StartupTaskConfigStore startup_configs;
    e8::TaskRegistry registry;
};

TestData::TestData()
    : history(kHistoryPeristencePath), startup_configs(kStartupListPeristencePath),
      registry(&history, &startup_configs) {}

TestData::~TestData() {
    remove(kHistoryPeristencePath);
    remove(kStartupListPeristencePath);
}

} // namespace

bool TaskLifeCycleTest() {
    TestData test_data;

    // Registers a new task launch with config ID "test_config" and task ID "test_task".
    e8::LaunchConfig launch_config;
    launch_config.set_config_id("test_config");
    launch_config.set_binary_directory(e8::LaunchConfig::BUILD);
    launch_config.set_binary_name("test_binary");
    (*launch_config.mutable_args())["key"] = "value";
    launch_config.set_startup(true);

    e8::TaskBasicInfo task_info = test_data.registry.RegisterNewTask(
        /*task_id=*/"test_task", /*process_id=*/1234, launch_config,
        /*stdout_file_name=*/"./test_stdout.log",
        /*stderr_file_name=*/"./test_stderr.log",
        /*stdall_file_name=*/"./test_stdall.log");
    TEST_CONDITION(task_info.task_id() == "test_task");
    TEST_CONDITION(task_info.launch_timestamp_millis() > 0);
    TEST_CONDITION(task_info.termination_timestamp_millis() == -1);
    TEST_CONDITION(task_info.launch_config().DebugString() == launch_config.DebugString());
    TEST_CONDITION(task_info.stdout_file_name() == "./test_stdout.log");
    TEST_CONDITION(task_info.stderr_file_name() == "./test_stderr.log");
    TEST_CONDITION(task_info.stdall_file_name() == "./test_stdall.log");

    // Waits for the task to be registered.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Checks if the test_task is added to the running list.
    std::unordered_map<pid_t, e8::TaskBasicInfo> const &running_tasks =
        test_data.registry.RunningTasks().List();
    TEST_CONDITION(running_tasks.size() == 1);
    TEST_CONDITION(running_tasks.find(1234) != running_tasks.end());
    TEST_CONDITION(running_tasks.at(1234).DebugString() == task_info.DebugString());

    // Checks task ID query.
    std::optional<std::pair<pid_t, e8::TaskBasicInfo>> find_by_task_id_result =
        test_data.registry.RunningTasks().FindByTaskId("test_task");
    TEST_CONDITION(find_by_task_id_result.has_value());
    TEST_CONDITION(find_by_task_id_result->first == 1234);
    TEST_CONDITION(find_by_task_id_result->second.DebugString() == task_info.DebugString());

    // Checks launch configuration ID query.
    std::optional<std::pair<pid_t, e8::TaskBasicInfo>> find_by_config_id_result =
        test_data.registry.RunningTasks().FindByLaunchConfigId("test_config");
    TEST_CONDITION(find_by_config_id_result.has_value());
    TEST_CONDITION(find_by_config_id_result->first == 1234);
    TEST_CONDITION(find_by_config_id_result->second.DebugString() == task_info.DebugString());

    // Terminates the task then check if it's kicked out from the running list.
    test_data.registry.AddTermination(1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::unordered_map<pid_t, e8::TaskBasicInfo> const &running_tasks2 =
        test_data.registry.RunningTasks().List();
    TEST_CONDITION(running_tasks2.empty());

    return true;
}

bool TerminationBeforePreviousRegistrationTest() {
    TestData test_data;

    // Terminates a task before it's launched.
    test_data.registry.AddTermination(1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    e8::LaunchConfig launch_config;
    launch_config.set_config_id("test_config");
    launch_config.set_binary_directory(e8::LaunchConfig::BUILD);
    launch_config.set_binary_name("test_binary");
    (*launch_config.mutable_args())["key"] = "value";
    launch_config.set_startup(true);

    test_data.registry.RegisterNewTask(
        /*task_id=*/"test_task", /*process_id=*/1234, launch_config,
        /*stdout_file_name=*/"./test_stdout.log",
        /*stderr_file_name=*/"./test_stderr.log",
        /*stdall_file_name=*/"./test_stdall.log");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Since the task is supposed to be terminated, it should not appear in the running list.
    std::unordered_map<pid_t, e8::TaskBasicInfo> const &running_tasks =
        test_data.registry.RunningTasks().List();
    TEST_CONDITION(running_tasks.empty());

    return true;
}

bool RegistrationBeforePreviousTerminationTest() {
    TestData test_data;

    // Adds a supposedly terminated old task to the registry.
    e8::LaunchConfig launch_config_previous;
    launch_config_previous.set_config_id("test_config_previous");
    launch_config_previous.set_binary_directory(e8::LaunchConfig::BUILD);
    launch_config_previous.set_binary_name("test_binary_previous");
    (*launch_config_previous.mutable_args())["key"] = "value";
    launch_config_previous.set_startup(true);

    test_data.registry.RegisterNewTask(
        /*task_id=*/"test_task_previous", /*process_id=*/1234, launch_config_previous,
        /*stdout_file_name=*/"./test_stdout_previous.log",
        /*stderr_file_name=*/"./test_stderr_previous.log",
        /*stdall_file_name=*/"./test_stdall_previous.log");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Adds a new task to the registry but shares the same process ID.
    e8::LaunchConfig launch_config_now;
    launch_config_now.set_config_id("test_config");
    launch_config_now.set_binary_directory(e8::LaunchConfig::BUILD);
    launch_config_now.set_binary_name("test_binary");
    (*launch_config_now.mutable_args())["key"] = "value";
    launch_config_now.set_startup(true);

    test_data.registry.RegisterNewTask(
        /*task_id=*/"test_task", /*process_id=*/1234, launch_config_now,
        /*stdout_file_name=*/"./test_stdout.log",
        /*stderr_file_name=*/"./test_stderr.log",
        /*stdall_file_name=*/"./test_stdall.log");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::unordered_map<pid_t, e8::TaskBasicInfo> const &running_tasks =
        test_data.registry.RunningTasks().List();
    TEST_CONDITION(running_tasks.size() == 1);
    TEST_CONDITION(running_tasks.find(1234) != running_tasks.end());
    TEST_CONDITION(running_tasks.at(1234).task_id() == "test_task_previous");

    // Notifies the registry that the old task was termianted.
    test_data.registry.AddTermination(1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Checks that only the new task is in the running list.
    std::unordered_map<pid_t, e8::TaskBasicInfo> const &running_tasks2 =
        test_data.registry.RunningTasks().List();
    TEST_CONDITION(running_tasks2.size() == 1);
    TEST_CONDITION(running_tasks2.find(1234) != running_tasks.end());
    TEST_CONDITION(running_tasks2.at(1234).task_id() == "test_task");

    return true;
}

bool StorageMutationTest() {
    TestData test_data;

    // Adds a startup task to the registry.
    e8::LaunchConfig launch_config;
    launch_config.set_config_id("test_config");
    launch_config.set_binary_directory(e8::LaunchConfig::BUILD);
    launch_config.set_binary_name("test_binary");
    (*launch_config.mutable_args())["key"] = "value";
    launch_config.set_startup(true);

    test_data.registry.RegisterNewTask(
        /*task_id=*/"test_task", /*process_id=*/1234, launch_config,
        /*stdout_file_name=*/"./test_stdout.log",
        /*stderr_file_name=*/"./test_stderr.log",
        /*stdall_file_name=*/"./test_stdall.log");

    // Checks the startup list has the launch configuration.
    google::protobuf::RepeatedPtrField<e8::LaunchConfig> startup_list =
        test_data.startup_configs.List();
    TEST_CONDITION(startup_list.size() == 1);
    TEST_CONDITION(startup_list[0].DebugString() == launch_config.DebugString());

    // Checks the history log has the task's information.
    std::vector<e8::TaskBasicInfo> history = test_data.history.List();
    TEST_CONDITION(history.size() == 1);
    TEST_CONDITION(history[0].task_id() == "test_task");
    TEST_CONDITION(history[0].launch_config().DebugString() == launch_config.DebugString());
    TEST_CONDITION(history[0].launch_timestamp_millis() > 0);
    TEST_CONDITION(history[0].termination_timestamp_millis() == -1);
    TEST_CONDITION(history[0].stdout_file_name() == "./test_stdout.log");
    TEST_CONDITION(history[0].stderr_file_name() == "./test_stderr.log");
    TEST_CONDITION(history[0].stdall_file_name() == "./test_stdall.log");

    // Terminates the task and the history should be updated with its termination timestamp.
    test_data.registry.AddTermination(1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    history = test_data.history.List();
    TEST_CONDITION(history.size() == 1);
    TEST_CONDITION(history[0].termination_timestamp_millis() > 0);
    TEST_CONDITION(history[0].termination_timestamp_millis() >
                   history[0].launch_timestamp_millis());

    return true;
}

int main() {
    e8::BeginTestSuite("task_registry");
    e8::RunTest("TaskLifeCycleTest", TaskLifeCycleTest);
    e8::RunTest("TerminationBeforePreviousRegistrationTest",
                TerminationBeforePreviousRegistrationTest);
    e8::RunTest("RegistrationBeforePreviousTerminationTest",
                RegistrationBeforePreviousTerminationTest);
    e8::RunTest("StorageMutationTest", StorageMutationTest);
    e8::EndTestSuite();
    return 0;
}
