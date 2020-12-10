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
#include <chrono>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>
#include <semaphore.h>
#include <thread>
#include <unordered_map>
#include <vector>

#include "message_queue/message_queue/module/message_queue_store.h"

namespace e8 {
namespace {

static MessageQueueStore gMessageQueueStore;

} // namespace

MessageQueueStore::MessageQueue::MessageQueue() { sem_init(&queue_resource_count, 0, 0); }

MessageQueueStore::MessageQueue::~MessageQueue() { sem_destroy(&queue_resource_count); }

MessageQueueStore::MessageQueue *MessageQueueStore::FetchQueue(MessageKey const key) {
    MessageQueue *queue;

    map_lock_.lock_shared();
    auto read_it = queues_.find(key);
    if (read_it != queues_.end()) {
        queue = read_it->second.get();
        map_lock_.unlock_shared();
    } else {
        map_lock_.unlock_shared();

        map_lock_.lock();
        auto write_it = queues_.insert(std::make_pair(key, std::make_shared<MessageQueue>())).first;
        queue = write_it->second.get();
        map_lock_.unlock();
    }

    assert(queue != nullptr);
    return queue;
}

void MessageQueueStore::Enqueue(MessageKey const key, RealTimeMessage const &message) {
    MessageQueue *message_queue = FetchQueue(key);

    message_queue->queue_lock.lock();
    message_queue->queue.push_back(message);
    message_queue->queue_lock.unlock();

    sem_post(&message_queue->queue_resource_count);
}

MessageQueueStore::MessageQueue *MessageQueueStore::BeginBlockingDequeue(MessageKey const key,
                                                                         int const wait_for_secs,
                                                                         RealTimeMessage *message) {
    MessageQueue *message_queue = FetchQueue(key);

    int rc;
    if (wait_for_secs > 0) {
        std::time_t expiry_timestamp;
        std::time(&expiry_timestamp);
        expiry_timestamp += wait_for_secs;

        timespec ts;
        ts.tv_sec = expiry_timestamp;
        ts.tv_nsec = 0;
        rc = sem_timedwait(&message_queue->queue_resource_count, &ts);
    } else {
        rc = sem_wait(&message_queue->queue_resource_count);
    }

    if (rc == -1 && errno == ETIMEDOUT) {
        return nullptr;
    }

    assert(rc == 0);

    message_queue->queue_lock.lock();
    *message = message_queue->queue.front();

    return message_queue;
}

void MessageQueueStore::EndBlockingDequeue(MessageQueue *message_queue, bool dequeue) {
    assert(message_queue != nullptr);

    if (dequeue) {
        message_queue->queue.pop_front();
    } else {
        sem_post(&message_queue->queue_resource_count);
    }
    message_queue->queue_lock.unlock();
}

std::vector<RealTimeMessage> MessageQueueStore::ListQueue(MessageKey const key) {
    MessageQueue *message_queue = FetchQueue(key);

    message_queue->queue_lock.lock();
    std::vector<RealTimeMessage> messages{message_queue->queue.begin(), message_queue->queue.end()};
    message_queue->queue_lock.unlock();

    return messages;
}

void MessageQueueStore::Clear() {
    map_lock_.lock();
    queues_.clear();
    map_lock_.unlock();
}

MessageQueueStats MessageQueueStore::QueueStats() {
    MessageQueueStats stats;

    map_lock_.lock();

    stats.set_total_num_queues(queues_.size());

    for (auto const &[key, queue] : queues_) {
        unsigned length = queue->queue.size();

        if (length == 0) {
            stats.set_num_queues_length_0(stats.num_queues_length_0() + 1);
        } else if (length >= 1 && length <= 10) {
            stats.set_num_queues_length_1_10(stats.num_queues_length_1_10() + 1);
        } else if (length >= 11 && length <= 100) {
            stats.set_num_queues_length_11_100(stats.num_queues_length_11_100() + 1);
        } else if (length >= 101 && length <= 1000) {
            stats.set_num_queues_length_101_1000(stats.num_queues_length_101_1000() + 1);
        } else {
            stats.set_num_queues_length_gte_1001(stats.num_queues_length_gte_1001() + 1);
        }
    }

    map_lock_.unlock();

    return stats;
}

MessageQueueStore *MessageQueueStoreInstance() { return &gMessageQueueStore; }

} // namespace e8
