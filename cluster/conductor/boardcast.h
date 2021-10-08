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

#include "cluster/conductor/condutor.h"
#include "proto_cc/cluster.pb.h"
#include "proto_cc/machine.pb.h"

namespace e8 {

/**
 * @brief BoardcastRevision Boardcasts the specified revision to the specified set of machines.
 *
 * @param revision The revision to be boardcasted.
 * @param target_machines The set of machines to boardcast the revision to.
 * @param rate At what proportion the revision is sent to the set of machines simultaneously.
 * @param this_conductor The conductor instance the client of this function is holding. When the
 * conductor adivces not to boardcast, this function will terminate as soon as possible and returns
 * false to indicate the boardcast was dropped.
 * @param unsuccessful_machines The set of machines that failed to apply the specified revision,
 * either because of network problems or a failed ApplyClusterMapRevision() RPC.
 * @return true iff. the revision has boardcasted to the target machines without it being dropped by
 * the conductor.
 */
bool BoardcastRevision(ClusterMapRevision const &revision,
                       std::vector<Machine> const &target_machines, float rate,
                       ClusterRevisionConductorInterface const &this_conductor,
                       std::vector<Machine> *unsuccessful_machines);

/**
 * @brief BoardcastRevisionWithRetry The same as the above excepts it takes the failed machines and
 * retry the revision upon them for num_retries times.
 */
bool BoardcastRevisionWithRetry(ClusterMapRevision const &revision,
                                std::vector<Machine> target_machines, float rate,
                                ClusterRevisionConductorInterface const &this_conductor,
                                unsigned num_retries, std::vector<Machine> *unsuccessful_machines);

} // namespace e8

#endif // CLUSTER_CONDUCTOR_BOARDCAST_H
