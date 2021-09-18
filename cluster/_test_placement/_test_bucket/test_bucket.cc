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

#include <memory>
#include <optional>
#include <unordered_set>

#include "cluster/placement/bucket.h"
#include "cluster/placement/bucket_list.h"
#include "cluster/placement/bucket_straw.h"
#include "cluster/placement/bucket_tree.h"
#include "cluster/placement/bucket_uniform.h"
#include "cluster/placement/capability.h"
#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "common/unit_test_util/unit_test_util.h"
#include "proto_cc/bucket.pb.h"

namespace {

/**
 * @brief WillSelect Records all the nodes we have visisted from the bucket by placing the specified
 * resource within the specified number of tolerated failures.
 */
std::unordered_set<e8::ClusterTreeNodeLabel> WillSelect(e8::ResourceDescriptor const &resource,
                                                        e8::BucketInterface *bucket,
                                                        unsigned max_failure_tolerance) {
    std::unordered_set<e8::ClusterTreeNodeLabel> visited;

    for (unsigned f = 0; f < max_failure_tolerance; ++f) {
        std::optional<e8::ClusterTreeNodeLabel> child = bucket->Select(resource, /*replica=*/0, f);
        if (child.has_value()) {
            visited.insert(*child);
        }
    }

    return visited;
}

bool GeneralTest(e8::BucketInterface *bucket, bool test_capability_distribution) {
    // 5 different resources with varying hardware capability requirements.
    e8::ResourceDescriptor cpu_hungry_resource(/*key=*/"a");
    *cpu_hungry_resource.required_capabilities.mutable_cpu() =
        e8::CapabilityFixedPointFromFloat(4.0f);

    e8::ResourceDescriptor ram_hungry_resource(/*key=*/"b");
    *ram_hungry_resource.required_capabilities.mutable_ram() =
        e8::CapabilityFixedPointFromFloat(4.0f);

    e8::ResourceDescriptor storage_hungry_resource(/*key=*/"c");
    *storage_hungry_resource.required_capabilities.mutable_storage() =
        e8::CapabilityFixedPointFromFloat(4.0f);

    e8::ResourceDescriptor coral_hungry_resource(/*key=*/"d");
    *coral_hungry_resource.required_capabilities.mutable_coral() =
        e8::CapabilityFixedPointFromFloat(4.0f);

    e8::ResourceDescriptor dont_care_resource(/*key=*/"e");

    // The bucket should be empty at this moment.
    std::optional<e8::ClusterTreeNodeLabel> child =
        bucket->Select(cpu_hungry_resource, /*replica=*/0, /*num_failures=*/0);
    TEST_CONDITION(!child.has_value());
    child = bucket->Select(ram_hungry_resource, /*replica=*/0, /*num_failures=*/0);
    TEST_CONDITION(!child.has_value());
    child = bucket->Select(storage_hungry_resource, /*replica=*/0, /*num_failures=*/0);
    TEST_CONDITION(!child.has_value());
    child = bucket->Select(coral_hungry_resource, /*replica=*/0, /*num_failures=*/0);
    TEST_CONDITION(!child.has_value());
    child = bucket->Select(dont_care_resource, /*replica=*/0, /*num_failures=*/0);
    TEST_CONDITION(!child.has_value());

    // 4 different nodes equiped with varying hardware capability
    e8::ClusterTreeNodeLabel high_performance_cpu_node = e8::AllocateClusterTreeNodeLabel();
    e8::WeightedCapabilities high_performance_cpu_capabilities;
    *high_performance_cpu_capabilities.mutable_cpu() = e8::CapabilityFixedPointFromFloat(4.0f);
    *high_performance_cpu_capabilities.mutable_storage() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_performance_cpu_capabilities.mutable_coral() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_performance_cpu_capabilities.mutable_ram() = e8::CapabilityFixedPointFromFloat(0.0f);

    e8::ClusterTreeNodeLabel high_ram_capacity_node = e8::AllocateClusterTreeNodeLabel();
    e8::WeightedCapabilities high_ram_capacity_capabilities;
    *high_ram_capacity_capabilities.mutable_cpu() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_ram_capacity_capabilities.mutable_ram() = e8::CapabilityFixedPointFromFloat(4.0f);
    *high_ram_capacity_capabilities.mutable_storage() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_ram_capacity_capabilities.mutable_coral() = e8::CapabilityFixedPointFromFloat(0.0f);

    e8::ClusterTreeNodeLabel high_storage_capacity_node = e8::AllocateClusterTreeNodeLabel();
    e8::WeightedCapabilities high_storage_capacity_capabilities;
    *high_storage_capacity_capabilities.mutable_cpu() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_storage_capacity_capabilities.mutable_ram() = e8::CapabilityFixedPointFromFloat(0.0f);
    *high_storage_capacity_capabilities.mutable_storage() = e8::CapabilityFixedPointFromFloat(4.0f);
    *high_storage_capacity_capabilities.mutable_coral() = e8::CapabilityFixedPointFromFloat(0.0f);

    e8::ClusterTreeNodeLabel coral_node = e8::AllocateClusterTreeNodeLabel();
    e8::WeightedCapabilities coral_capabilities;
    *coral_capabilities.mutable_cpu() = e8::CapabilityFixedPointFromFloat(0.0f);
    *coral_capabilities.mutable_ram() = e8::CapabilityFixedPointFromFloat(0.0f);
    *coral_capabilities.mutable_storage() = e8::CapabilityFixedPointFromFloat(0.0f);
    *coral_capabilities.mutable_coral() = e8::CapabilityFixedPointFromFloat(4.0f);

    // Adds nodes to the bucket, and checks that the bucket does not allow double addings/overrides.
    bool added = bucket->AddChild(high_performance_cpu_node);
    TEST_CONDITION(added);
    bucket->AddCapabilitiesFor(high_performance_cpu_node, high_performance_cpu_capabilities);

    added = bucket->AddChild(high_ram_capacity_node);
    TEST_CONDITION(added);
    bucket->AddCapabilitiesFor(high_ram_capacity_node, high_ram_capacity_capabilities);

    added = bucket->AddChild(high_storage_capacity_node);
    TEST_CONDITION(added);
    bucket->AddCapabilitiesFor(high_storage_capacity_node, high_storage_capacity_capabilities);

    added = bucket->AddChild(coral_node);
    TEST_CONDITION(added);
    bucket->AddCapabilitiesFor(coral_node, coral_capabilities);

    added = bucket->AddChild(high_performance_cpu_node);
    TEST_CONDITION(!added);
    added = bucket->AddChild(high_ram_capacity_node);
    TEST_CONDITION(!added);
    added = bucket->AddChild(high_storage_capacity_node);
    TEST_CONDITION(!added);
    added = bucket->AddChild(coral_node);
    TEST_CONDITION(!added);

    // Checks if the bucket distribute resource according to the hardware capability setting and
    // requirement.
    if (test_capability_distribution) {
        child = bucket->Select(cpu_hungry_resource, /*replica=*/0, /*num_failures=*/0);
        TEST_CONDITION(child.has_value());
        TEST_CONDITION(*child == high_performance_cpu_node);

        child = bucket->Select(ram_hungry_resource, /*replica=*/0, /*num_failures=*/0);
        TEST_CONDITION(child.has_value());
        TEST_CONDITION(*child == high_ram_capacity_node);

        child = bucket->Select(storage_hungry_resource, /*replica=*/0, /*num_failures=*/0);
        TEST_CONDITION(child.has_value());
        TEST_CONDITION(*child == high_storage_capacity_node);

        child = bucket->Select(coral_hungry_resource, /*replica=*/0, /*num_failures=*/0);
        TEST_CONDITION(child.has_value());
        TEST_CONDITION(*child == coral_node);
    }

    // Checks if the bucket iterates through all nodes by repeatedly placing a don't-care-resource.
    std::unordered_set<e8::ClusterTreeNodeLabel> selected =
        WillSelect(dont_care_resource, bucket, /*max_failure_tolerance=*/4 * 4);
    TEST_CONDITION(selected.size() == 4);
    TEST_CONDITION(selected.find(high_performance_cpu_node) != selected.end());
    TEST_CONDITION(selected.find(high_ram_capacity_node) != selected.end());
    TEST_CONDITION(selected.find(high_storage_capacity_node) != selected.end());
    TEST_CONDITION(selected.find(coral_node) != selected.end());

    // Graduately removing nodes and we should be able to select less nodes .
    bool removed = bucket->RemoveChild(high_ram_capacity_node);
    TEST_CONDITION(removed == true);
    removed = bucket->RemoveChild(high_ram_capacity_node);
    TEST_CONDITION(removed == false);
    selected = WillSelect(dont_care_resource, bucket, /*max_failure_tolerance=*/4 * 4);
    TEST_CONDITION(selected.size() == 3);
    TEST_CONDITION(selected.find(high_performance_cpu_node) != selected.end());
    TEST_CONDITION(selected.find(high_storage_capacity_node) != selected.end());
    TEST_CONDITION(selected.find(coral_node) != selected.end());

    removed = bucket->RemoveChild(high_performance_cpu_node);
    TEST_CONDITION(removed == true);
    removed = bucket->RemoveChild(high_performance_cpu_node);
    TEST_CONDITION(removed == false);
    selected = WillSelect(dont_care_resource, bucket, /*max_failure_tolerance=*/4 * 4);
    TEST_CONDITION(selected.size() == 2);
    TEST_CONDITION(selected.find(high_storage_capacity_node) != selected.end());
    TEST_CONDITION(selected.find(coral_node) != selected.end());

    removed = bucket->RemoveChild(coral_node);
    TEST_CONDITION(removed == true);
    removed = bucket->RemoveChild(coral_node);
    TEST_CONDITION(removed == false);
    selected = WillSelect(dont_care_resource, bucket, /*max_failure_tolerance=*/4 * 4);
    TEST_CONDITION(selected.size() == 1);
    TEST_CONDITION(selected.find(high_storage_capacity_node) != selected.end());

    removed = bucket->RemoveChild(high_storage_capacity_node);
    TEST_CONDITION(removed == true);
    removed = bucket->RemoveChild(high_storage_capacity_node);
    TEST_CONDITION(removed == false);
    selected = WillSelect(dont_care_resource, bucket, /*max_failure_tolerance=*/4 * 4);
    TEST_CONDITION(selected.empty());

    return true;
}

} // namespace

bool UniformBucketTest() {
    e8::UniformBucketData data;
    data.set_prime(13);

    e8::UniformBucket bucket(data);
    return GeneralTest(&bucket, /*test_capability_distribution=*/false);
}

bool ListBucketTest() {
    e8::ListBucketData data;

    e8::ListBucket bucket(data, std::make_unique<e8::MostDemandingCapabilityScore>());
    return GeneralTest(&bucket, /*test_capability_distribution=*/true);
}

bool TreeBucketTest() {
    e8::TreeBucketData data;

    e8::TreeBucket bucket(data, std::make_unique<e8::MostDemandingCapabilityScore>());
    return GeneralTest(&bucket, /*test_capability_distribution=*/true);
}

bool StrawBucketTest() {
    e8::StrawBucketData data;
    data.set_weight_function(e8::StrawBucketData::LINEAR);

    e8::StrawBucket bucket(data, std::make_unique<e8::MostDemandingCapabilityScore>());
    return GeneralTest(&bucket, /*test_capability_distribution=*/true);
}

int main() {
    e8::BeginTestSuite("placement_bucket");
    e8::RunTest("UniformBucketTest", UniformBucketTest);
    e8::RunTest("ListBucketTest", ListBucketTest);
    e8::RunTest("TreeBucketTest", TreeBucketTest);
    e8::RunTest("StrawBucketTest", StrawBucketTest);
    e8::EndTestSuite();
    return 0;
}
