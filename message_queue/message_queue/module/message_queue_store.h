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

#ifndef MESSAGE_QUEUE_STORE_H
#define MESSAGE_QUEUE_STORE_H

#include <cstdint>
#include <ctime>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>
#include <semaphore.h>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "message_queue/common/entity.h"
#include "proto_cc/message_queue_stats.pb.h"
#include "proto_cc/real_time_message.pb.h"

namespace e8 {

/**
 * @brief The MessageQueueStore class A thread-safe FIFO message queue store. It stores a set of
 * message queues identfied by a unique key.
 */
class MessageQueueStore {
  public:
    struct MessageQueue {
        MessageQueue();
        ~MessageQueue();

        std::deque<RealTimeMessage> queue;

        std::mutex queue_lock;
        sem_t queue_resource_count;
    };

    /**
     * @brief Enqueue Add a new message to the queue pointed by the parameter key. If there are
     * readers calling BlockingDequeue on an empty queue, this operation will unblock one of the
     * readers.
     *
     * @param key A unique ID pointing to the queue to add message to.
     * @param message Message to be added.
     */
    void Enqueue(MessageKey const key, RealTimeMessage const &message);

    /**
     * @brief BlockingDequeue Read the oldest element from the queue pointed to by the key. If the
     * queue is empty, this function will block until it becomes non-empty. If the element exists,
     * this function will exclusively block concurrent access until EndBlockingDequeue() is called.
     *
     * @param key A unique ID pointing to the queue to read the message from.
     * @param wait_for_secs The number of seconds to wait before returning an empty queue if there
     * isn't anything coming into the queue for at least this duration.
     * @param message returns the The oldest message from the queue.
     * @return A pointer to the message queue pointed to by the message key.
     */
    MessageQueue *BeginBlockingDequeue(MessageKey const key, int const wait_for_secs,
                                       RealTimeMessage *message);

    /**
     * @brief EndBlockingDequeue Unblock the queue's concurrent access and potentially remove the
     * oldest element depending on the "dequeue" argument.
     *
     * @param message_queue Pointer to the queue to be unblocked the concurrent access.
     * @param dequeue Whether or not to remove the oldest element before the concurrent access is
     * recovered.
     */
    void EndBlockingDequeue(MessageQueue *message_queue, bool dequeue);

    /**
     * @brief ListQueue Returns all the messages in the queue pointed by the key.
     */
    std::vector<RealTimeMessage> ListQueue(MessageKey const key);

    /**
     * @brief Clear Delete all the message queues.
     */
    void Clear();

    /**
     * @brief QueueStats Calculate the current statistics of all queues.
     */
    MessageQueueStats QueueStats();

  private:
    MessageQueue *FetchQueue(MessageKey const key);

    std::unordered_map<MessageKey, std::shared_ptr<MessageQueue>> queues_;
    std::shared_mutex map_lock_;
};

/**
 * @brief MessageQueueStoreInstance Get the singleton MessageQueueStore instance.
 */
MessageQueueStore *MessageQueueStoreInstance();

} // namespace e8

#endif // MESSAGE_QUEUE_STORE_H
