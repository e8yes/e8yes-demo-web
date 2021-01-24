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

#ifndef GAME_INSTANCE_CONTAINER_H
#define GAME_INSTANCE_CONTAINER_H

#include <cstdint>
#include <memory>

#include "gomoku/game/game.h"

namespace e8 {

/**
 * @brief The GameInstanceContainer class Schedules and runs game instances.
 */
class GameInstanceContainer {
  public:
    struct GameInstanceContainerInternal;

    using ScheduleId = int64_t;

    /**
     * @brief GameInstanceContainer Constructs a container that runs max_concurrent_games number of
     * game instances at a time.
     */
    GameInstanceContainer(unsigned max_concurrent_games);
    GameInstanceContainer(GameInstanceContainer const &) = delete;
    GameInstanceContainer(GameInstanceContainer &&) = delete;

    /**
     * @brief ScheduleForRun Add a new game instance into the container and schedule to run whenever
     * hardware resource is available. The hardware resource limit is set in the constructor by the
     * argument max_concurrent_games.
     */
    void ScheduleToRun(ScheduleId schedule_id, std::unique_ptr<GomokuGame> &&game);

    /**
     * @brief ScheduledGame Retrieve the game instance by the schedule ID. If the schedule ID is
     * invalid or the game instance has finished running, it will return a nullptr.
     */
    std::shared_ptr<GomokuGame> ScheduledGame(ScheduleId id);

  private:
    std::unique_ptr<GameInstanceContainerInternal> pimpl_;
};

/**
 * @brief AllocateGameInstanceContainerScheduleId
 * @return
 */
GameInstanceContainer::ScheduleId AllocateGameInstanceContainerScheduleId();

/**
 * @brief DefaultGameContainer Returns a pointer to the global instance of a
 * GameInstanceContainer.
 */
GameInstanceContainer *DefaultGameInstanceContainer();

} // namespace e8

#endif // GAME_INSTANCE_CONTAINER_H
