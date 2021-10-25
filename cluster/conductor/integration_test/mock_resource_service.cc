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
#include <grpcpp/grpcpp.h>
#include <thread>
#include <vector>

#include "cluster/conductor/integration_test/mock_resource_service.h"
#include "cluster/placement/cluster_map.h"
#include "common/random/random_source.h"
#include "common/random/uniform_distribution.h"
#include "proto_cc/cluster_revision.pb.h"
#include "proto_cc/service_resource_worker.grpc.pb.h"
#include "proto_cc/service_resource_worker.pb.h"

namespace e8 {
namespace {

unsigned const kMinNumObjects = 1;
unsigned const kMaxNumObjects = 3;

} // namespace

MockResourceService::MockResourceService() {}

MockResourceService::~MockResourceService() {}

grpc::Status MockResourceService::ApplyClusterMapRevision(
    grpc::ServerContext * /*context*/, ApplyClusterMapRevisionRequest const *request,
    grpc::ServerWriter<ApplyClusterMapRevisionResponse> *writer) {
    ++rpc_count_;

    assert(request->has_revision());
    if (request->revision().from_version_epoch() != cluster_map_.Version()) {
        ApplyClusterMapRevisionResponse revision_version_mismatch_response;
        revision_version_mismatch_response.set_require_from_version_epoch(cluster_map_.Version());
        writer->Write(revision_version_mismatch_response);
        return grpc::Status::OK;
    }

    // Generates failure mode.
    int total_num_mock_objects = DrawUniformInts(kMinNumObjects, kMaxNumObjects + 1,
                                                 /*num_instances=*/1, &random_source_)[0];
    int should_fail_at_object;
    if (remaining_failure_count_ > 0) {
        should_fail_at_object = DrawUniformInts(kMinNumObjects, total_num_mock_objects,
                                                /*num_instances=*/1, &random_source_)[0];
        --remaining_failure_count_;
    } else {
        should_fail_at_object = -1;
    }

    // Sends messages according to the requested interval.
    ApplyClusterMapRevisionResponse response;
    response.set_require_from_version_epoch(-1);
    response.set_total_num_objects(total_num_mock_objects);

    for (int i = 0; i < total_num_mock_objects; ++i) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(request->status_update_interval_millis()));

        if (i == should_fail_at_object) {
            response.set_num_corrupted_objects(1);
            writer->Write(response);
            return grpc::Status::OK;
        }

        response.set_num_objects_processed(i + 1);
        writer->Write(response);
    }

    response.set_successful(true);
    writer->Write(response);

    // All mock objects have been successfully processed. Revises the cluster map.
    bool revisable = cluster_map_.Revise(request->revision());
    assert(revisable);

    return grpc::Status::OK;
}

ClusterMap const &MockResourceService::GetClusterMap() const { return cluster_map_; }

unsigned MockResourceService::RpcCount() const { return rpc_count_; }

void MockResourceService::AddFailures(unsigned num_failures) {
    remaining_failure_count_ += num_failures;
}

} // namespace e8
