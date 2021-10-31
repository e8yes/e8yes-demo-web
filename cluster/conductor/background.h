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

#ifndef CLUSTER_CONDUCTOR_BACKGROUND_H
#define CLUSTER_CONDUCTOR_BACKGROUND_H

#include <memory>

#include "cluster/conductor/conductor.h"
#include "common/random/random_source.h"
#include "common/thread/thread_pool.h"

namespace e8 {
namespace cluster_revision_background_internal {} // namespace cluster_revision_background_internal

/**
 * @brief The ClusterConductorBackground class The background task to continuely poll
 * work-in-progress revisions from the revision store and boardcast them to the cluster whenever the
 * conductor allows it to boardcast. If the conductor loses boardcast power either because of a
 * crash or network issue, and when there is some other conductor which gains the boardcast power,
 * its background task will pick up the revision and redo the boardcast. This ensures there is
 * always at least one agent to make the entire cluster in sync with the conductors.
 */
class ClusterConductorBackground : public TaskInterface {
  public:
    /**
     * @brief ClusterRevisionBackground This background task wraps on the revision conductor and
     * pushes revisions to the cluster when it's possible.
     */
    ClusterConductorBackground(ClusterRevisionConductorInterface *this_conductor);
    ~ClusterConductorBackground() override;

    void Run(TaskStorageInterface *) const override;

    bool DropResourceOnCompletion() const override;

    /**
     * @brief Shutdown Tells the background task to shut down. This function does not block.
     */
    void Shutdown();

  private:
    ClusterRevisionConductorInterface *const this_conductor_;
    std::unique_ptr<RandomSource> random_source_;
    bool done_;
};

} // namespace e8

#endif // CLUSTER_CONDUCTOR_BACKGROUND_H
