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

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <memory>
#include <thread>

namespace e8 {

/**
 * @brief The TaskStorageInterface class Serves as input and output of a task.
 */
class TaskStorageInterface {
  public:
    virtual ~TaskStorageInterface();
};

/**
 * @brief The TaskInterface class Generic task to be scheduled and invoked.
 */
class TaskInterface {
  public:
    virtual ~TaskInterface();

    /**
     * @brief Run When the task is being invoked, this function will be called with the provided
     * task storage.
     * @param storage If no task storage is provided, this parameter will be a nullptr.
     */
    virtual void Run(TaskStorageInterface *storage) const = 0;

    /**
     * @brief DropResourceOnCompletion If this is true, the task as well as its storage will be
     * destroyed as long as the task is finished.
     */
    virtual bool DropResourceOnCompletion() const = 0;
};

/**
 * @brief The ThreadPool class A pool of native worker threads invoking tasks in parallel.
 */
class ThreadPool {
  public:
    /**
     * @brief ThreadPool Construct a thread pool with a specified number of worker threads.
     */
    ThreadPool(unsigned num_workers = std::thread::hardware_concurrency());
    ~ThreadPool();

    /**
     * @brief Schedule Add the task into a queue hence the task will be scheduled in a
     * first-come-first-serve manner. One can make multiple Run() call with the same task but
     * different task_data, so that a single task can process different data.
     *
     * @param task Task to be scheduled and invoked.
     * @param task_data Task data to pass in with the given task.
     */
    void Schedule(std::shared_ptr<TaskInterface> task,
                  std::unique_ptr<TaskStorageInterface> &&task_data = nullptr);

    /**
     * @brief WaitForNextCompleted Wait for the next task to be completed and return the tasks'
     * data. If there isn't and won't be any task being scheduled, calling this function will result
     * in dead lock.
     */
    std::unique_ptr<TaskStorageInterface> WaitForNextCompleted();

    /**
     * @brief NumWorkers The number of thread workers in this pool.
     */
    unsigned NumWorkers() const;

  public:
    struct ThreadPoolInternal;

    std::unique_ptr<ThreadPoolInternal> pimpl_;
};

} // namespace e8

#endif // THREAD_POOL_H
