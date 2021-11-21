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

#include <cassert>
#include <chrono>
#include <fstream>
#include <google/protobuf/text_format.h>
#include <streambuf>
#include <string>
#include <thread>

#include "cluster/conductor/instance.h"
#include "common/flags/parse_flags.h"
#include "proto_cc/cluster_conductor.pb.h"

namespace {

constexpr char const *kClusterConductorConfigPath = "config_path";

e8::ClusterConductorConfig LoadConductorConfig(std::string const &file) {
    std::ifstream config_file(file, std::ios::in);
    assert(config_file.is_open());

    std::string content((std::istreambuf_iterator<char>(config_file)),
                        (std::istreambuf_iterator<char>()));

    e8::ClusterConductorConfig config;
    bool success = google::protobuf::TextFormat::ParseFromString(content, &config);
    assert(success);

    return config;
}

} // namespace

int main(int argc, char *argv[]) {
    e8::Argv(argc, argv);
    std::string replication_config_path =
        e8::ReadFlag(kClusterConductorConfigPath, std::string(), e8::FromString<std::string>);
    assert(!replication_config_path.empty());

    e8::ClusterConductorConfig config = LoadConductorConfig(replication_config_path);
    e8::ConductorInstance conductor_instance(config);

    // TODO: creates ways to gracefully terminate this program.
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };

    return 0;
}
