/**
 * e8yes demo web server.
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

#ifndef MESSAGE_QUEUE_STORE_H
#define MESSAGE_QUEUE_STORE_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <semaphore.h>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "proto_cc/real_time_message.pb.h"

namespace e8 {

using MessageKey = int64_t;

/**
 * @brief The MessageQueueStore class A thread-safe FIFO message queue store. It stores a set of
 * message queues identfied by a unique key.
 */
class MessageQueueStore {
  public:
    /**
     * @brief Enqueue Add a new message to the queue pointed by the parameter key. If there are
     * readers calling BlockingDequeue on an empty queue, this operation will unblock one of the
     * readers.
     *
     * @param key A unique ID pointing to the queue to add message to.
     * @param message Message to be added.
     */
    void Enqueue(MessageKey key, RealTimeMessage const &message);

    /**
     * @brief BlockingDequeue Read and remove the oldest element from the queue pointed to by the
     * key. If the queue is empty, this function will block until it becomes non-empty.
     *
     * @param key A unique ID pointing to the queue to read and remove message from.
     * @return The oldest message from the queue.
     */
    RealTimeMessage BlockingDequeue(MessageKey key);

    /**
     * @brief Clear Delete all the message queues.
     */
    void Clear();

  private:
    struct MessageQueue {
        MessageQueue();
        ~MessageQueue();

        std::queue<RealTimeMessage> queue;
        std::mutex lock;
        sem_t sem;
    };

    MessageQueue *FetchQueue(MessageKey key);

    std::unordered_map<MessageKey, std::shared_ptr<MessageQueue>> queues_;
    std::shared_mutex map_lock_;
};

/**
 * @brief MessageQueueStoreInstance Get the singleton MessageQueueStore instance.
 */
MessageQueueStore *MessageQueueStoreInstance();

} // namespace e8

#endif // MESSAGE_QUEUE_STORE_H
