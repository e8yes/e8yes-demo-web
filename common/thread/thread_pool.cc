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
#include <mutex>
#include <queue>
#include <semaphore.h>
#include <thread>
#include <vector>

#include "common/thread/thread_pool.h"

namespace e8 {

struct TaskInfo {
    std::shared_ptr<TaskInterface> task;
    std::unique_ptr<TaskStorageInterface> task_data;

    TaskInfo(std::shared_ptr<TaskInterface> const &task,
             std::unique_ptr<TaskStorageInterface> &&task_data);
    TaskInfo(TaskInfo const &) = delete;
    TaskInfo(TaskInfo &&other);
};

TaskInfo::TaskInfo(std::shared_ptr<TaskInterface> const &task,
                   std::unique_ptr<TaskStorageInterface> &&task_data)
    : task(task), task_data(std::move(task_data)) {}

TaskInfo::TaskInfo(TaskInfo &&other) {
    std::swap(other.task, task);
    std::swap(other.task_data, task_data);
}

struct ThreadPool::ThreadPoolInternal {
    ThreadPoolInternal(unsigned num_workers);
    ~ThreadPoolInternal();

    std::vector<std::thread> worker_threads;

    sem_t task_resource;
    sem_t sink_resource;

    std::mutex source_lock;
    std::mutex sink_lock;

    std::queue<TaskInfo> tasks;
    std::queue<TaskInfo> completed_tasks;

    bool running = true;
};

template <typename OnFoundTaskFn>
bool HasTaskAndRun(ThreadPool::ThreadPoolInternal *this_, OnFoundTaskFn on_found_task_fn) {
    this_->source_lock.lock();

    if (this_->tasks.empty()) {
        this_->source_lock.unlock();
        return false;
    }

    TaskInfo info = std::move(this_->tasks.front());
    this_->tasks.pop();

    this_->source_lock.unlock();

    on_found_task_fn(std::move(info));

    return true;
}

void RunThreadPoolWorker(ThreadPool::ThreadPoolInternal *this_) {
    auto run_task_fn = [this_](TaskInfo &&task_info) {
        task_info.task->Run(task_info.task_data.get());

        if (!task_info.task->DropResourceOnCompletion()) {
            this_->sink_lock.lock();
            this_->completed_tasks.push(std::move(task_info));
            this_->sink_lock.unlock();

            sem_post(&this_->sink_resource);
        }
    };

    do {
        sem_wait(&this_->task_resource);

        while (HasTaskAndRun(this_, run_task_fn)) {
            // Do nothing.
        }
    } while (this_->running);
}

ThreadPool::ThreadPoolInternal::ThreadPoolInternal(unsigned num_workers) {
    sem_init(&task_resource, 0, 0);
    sem_init(&sink_resource, 0, 0);

    for (unsigned i = 0; i < num_workers; ++i) {
        worker_threads.push_back(std::thread(RunThreadPoolWorker, this));
    }
}

ThreadPool::ThreadPoolInternal::~ThreadPoolInternal() {
    running = false;

    for (unsigned i = 0; i < worker_threads.size(); ++i) {
        sem_post(&task_resource);
    }

    for (auto &worker : worker_threads) {
        worker.join();
    }

    sem_destroy(&task_resource);
    sem_destroy(&sink_resource);
}

TaskStorageInterface::~TaskStorageInterface() {}

TaskInterface::~TaskInterface() {}

ThreadPool::ThreadPool(unsigned num_workers)
    : pimpl_(std::make_unique<ThreadPoolInternal>(num_workers)) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::Schedule(std::shared_ptr<TaskInterface> task,
                          std::unique_ptr<TaskStorageInterface> &&task_data) {
    pimpl_->source_lock.lock();
    pimpl_->tasks.push(TaskInfo(task, std::move(task_data)));
    pimpl_->source_lock.unlock();

    sem_post(&pimpl_->task_resource);
}

std::unique_ptr<TaskStorageInterface> ThreadPool::WaitForNextCompleted() {
    sem_wait(&pimpl_->sink_resource);

    pimpl_->sink_lock.lock();

    assert(!pimpl_->completed_tasks.empty());
    TaskInfo task_info = std::move(pimpl_->completed_tasks.front());
    pimpl_->completed_tasks.pop();

    pimpl_->sink_lock.unlock();

    return std::move(task_info.task_data);
}

unsigned ThreadPool::NumWorkers() const { return pimpl_->worker_threads.size(); }

} // namespace e8
