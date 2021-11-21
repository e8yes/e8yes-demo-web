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

#ifndef CLUSTER_CONDUTOR_INSTANCE_H
#define CLUSTER_CONDUTOR_INSTANCE_H

#include <memory>

#include "proto_cc/cluster_conductor.pb.h"

namespace e8 {

/**
 * @brief The ConductorInstance class A replicated conductor instance. It serves as the "main
 * entrance" class of a cluster conductor node. It runs the background task to respond to and
 * process commands sent from the ClusterConductorClient.  It also runs the background task to
 * respond to machine failure events and revise the cluster to ensure resource safety. Finally, it
 * runs the background task to aggregate cluster revisions and boardcast them to keep the cluster in
 * sync.
 */
class ConductorInstance {
  public:
    /**
     * @brief ConductorInstance By the end of construction, all the background tasks are launched to
     * conduct the cluster.
     */
    ConductorInstance(ClusterConductorConfig const &config);
    ~ConductorInstance();

  private:
    struct ConductorInstanceImpl;

    std::unique_ptr<ConductorInstanceImpl> pimpl_;
};

} // namespace e8

#endif // CLUSTER_CONDUTOR_INSTANCE_H
