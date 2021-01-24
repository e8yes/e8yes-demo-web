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

#include <cstdint>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

#include "common/thread/thread_pool.h"
#include "gomoku/game/game.h"
#include "gomoku/game/game_instance_container.h"

namespace e8 {
namespace {

std::mutex gContainerPtrLock;
std::unique_ptr<GameInstanceContainer> gGameContainer;

struct GameRunnerStorage : public TaskStorageInterface {
    GameRunnerStorage(GameInstanceContainer::ScheduleId schedule_id, GomokuGame *game);

    GameInstanceContainer::ScheduleId const schedule_id;
    GomokuGame *const game;
};

GameRunnerStorage::GameRunnerStorage(GameInstanceContainer::ScheduleId schedule_id,
                                     GomokuGame *game)
    : schedule_id(schedule_id), game(game) {}

class GameRunnerTask : public TaskInterface {
  public:
    GameRunnerTask() = default;
    ~GameRunnerTask() override = default;

    void Run(TaskStorageInterface *storage) const override;

    bool DropResourceOnCompletion() const override;
};

void GameRunnerTask::Run(TaskStorageInterface *storage) const {
    if (storage == nullptr) {
        // Placeholder task.
        return;
    }
    static_cast<GameRunnerStorage *>(storage)->game->Start();
}

bool GameRunnerTask::DropResourceOnCompletion() const { return false; }

void CleanUpCompletedGames(ThreadPool *thread_pool,
                           std::unordered_map<GameInstanceContainer::ScheduleId,
                                              std::shared_ptr<GomokuGame>> *scheduled_games,
                           std::shared_mutex *lock, bool *cleanup_thread_running) {
    while (*cleanup_thread_running) {
        std::unique_ptr<TaskStorageInterface> storage = thread_pool->WaitForNextCompleted();

        if (storage == nullptr) {
            continue;
        }

        lock->lock();
        scheduled_games->erase(static_cast<GameRunnerStorage *>(storage.get())->schedule_id);
        lock->unlock();
    }
}

} // namespace

struct GameInstanceContainer::GameInstanceContainerInternal {
    GameInstanceContainerInternal(unsigned max_concurrent_games);
    ~GameInstanceContainerInternal();

    ThreadPool thread_pool;

    std::unordered_map<ScheduleId, std::shared_ptr<GomokuGame>> scheduled_games;
    std::shared_mutex lock;

    ScheduleId next_schedule_id;

    bool cleanup_thread_running;
    std::thread cleanup_thread;
};

GameInstanceContainer::GameInstanceContainerInternal::GameInstanceContainerInternal(
    unsigned max_concurrent_games)
    : thread_pool(max_concurrent_games), cleanup_thread_running(true),
      cleanup_thread(CleanUpCompletedGames, &thread_pool, &scheduled_games, &lock,
                     &cleanup_thread_running) {}

GameInstanceContainer::GameInstanceContainerInternal::~GameInstanceContainerInternal() {
    cleanup_thread_running = false;

    // Placeholder task to break out from the blacking wait.
    thread_pool.Schedule(std::make_shared<GameRunnerTask>(), /*task_data=*/nullptr);

    cleanup_thread.join();
}

GameInstanceContainer::GameInstanceContainer(unsigned max_concurrent_games)
    : pimpl_(std::make_unique<GameInstanceContainerInternal>(max_concurrent_games)) {}

GameInstanceContainer::ScheduleId
GameInstanceContainer::ScheduleToRun(std::unique_ptr<GomokuGame> &&game) {
    pimpl_->lock.lock();

    ScheduleId schedule_id = pimpl_->next_schedule_id++;
    auto [_, game_ptr] =
        *pimpl_->scheduled_games.insert(std::make_pair(schedule_id, std::move(game))).first;

    pimpl_->thread_pool.Schedule(std::make_shared<GameRunnerTask>(),
                                 std::make_unique<GameRunnerStorage>(schedule_id, game_ptr.get()));

    pimpl_->lock.unlock();

    return schedule_id;
}

std::shared_ptr<GomokuGame> GameInstanceContainer::ScheduledGame(ScheduleId id) {
    std::shared_ptr<GomokuGame> result;

    pimpl_->lock.lock_shared();

    auto it = pimpl_->scheduled_games.find(id);
    if (it != pimpl_->scheduled_games.end()) {
        result = it->second;
    } else {
        result = nullptr;
    }

    pimpl_->lock.unlock_shared();

    return result;
}

GameInstanceContainer *DefaultGameInstanceContainer() {
    gContainerPtrLock.lock();
    if (gGameContainer == nullptr) {
        gGameContainer = std::make_unique<GameInstanceContainer>(/*max_concurrent_games=*/4);
    }
    gContainerPtrLock.unlock();

    return gGameContainer.get();
}

} // namespace e8
