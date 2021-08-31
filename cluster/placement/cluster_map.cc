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

#include "cluster/placement/cluster_map.h"
#include "cluster/placement/common_types.h"
#include "third_party/uuid/uuid4.h"

namespace e8 {
namespace {

struct UuidState {
    UuidState() { uuid4_seed(&uuid_state); }
    UUID4_STATE_T uuid_state;
};

UuidState gUuidState;

} // namespace

ClusterTreeNodeLabel AllocateClusterTreeNodeLabel() {
    UUID4_T uuid;
    uuid4_gen(&gUuidState.uuid_state, &uuid);

    char uuid_string[UUID4_STR_BUFFER_SIZE];
    uuid4_to_s(uuid, uuid_string, sizeof(uuid_string));

    return ClusterTreeNodeLabel(uuid_string);
}

} // namespace e8
