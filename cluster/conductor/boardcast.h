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

#ifndef CLUSTER_CONDUCTOR_BOARDCAST_H
#define CLUSTER_CONDUCTOR_BOARDCAST_H

#include <vector>

#include "cluster/conductor/conductor.h"
#include "proto_cc/cluster_revision.pb.h"

namespace e8 {

/**
 * @brief BoardcastRevision Boardcasts revisions to the cluster to keep as many machines in-sync as
 * possible and keeps track of those that fail to get updated.
 *
 * @param revision_work
 * @param all_revisions
 * @param rate At what proportion the revision is sent to the set of machines simultaneously.
 * @param this_conductor The conductor instance the client of this function is holding. When the
 * conductor adivces not to boardcast, this function will terminate as soon as possible and returns
 * false to indicate the boardcast was dropped.
 * @param leftover_work
 * @return true iff. the function has gone through all the targets (regardless of failures) without
 * being dropped by the conductor.
 */
bool BoardcastRevision(ClusterRevisionWork const &revision_work,
                       std::vector<ClusterMapRevision> const &all_revisions, float rate,
                       ClusterRevisionConductorInterface const &this_conductor,
                       ClusterRevisionWork *leftover_work);

/**
 * @brief BoardcastRevisionWithRetry The same as the above excepts it takes those failed machines
 * and retry the revision upon them for num_retries times.
 */
bool BoardcastRevisionWithRetry(ClusterRevisionWork const &revision_work,
                                std::vector<ClusterMapRevision> const &all_revisions, float rate,
                                ClusterRevisionConductorInterface const &this_conductor,
                                unsigned num_retries, ClusterRevisionWork *leftover_work);

} // namespace e8

#endif // CLUSTER_CONDUCTOR_BOARDCAST_H
