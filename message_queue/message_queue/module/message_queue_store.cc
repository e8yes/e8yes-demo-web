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

#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <semaphore.h>
#include <thread>
#include <unordered_map>
#include <vector>

#include "message_queue/message_queue/module/message_queue_store.h"

namespace e8 {
namespace {

static MessageQueueStore gMessageQueueStore;

} // namespace

MessageQueueStore::MessageQueue::MessageQueue() { sem_init(&sem, 0, 0); }

MessageQueueStore::MessageQueue::~MessageQueue() { sem_destroy(&sem); }

MessageQueueStore::MessageQueue *MessageQueueStore::FetchQueue(MessageKey key) {
    map_lock_.lock_shared();

    auto it = queues_.find(key);
    if (it == queues_.end()) {
        map_lock_.lock();
        it = queues_.insert(std::make_pair(key, std::make_shared<MessageQueue>())).first;
        map_lock_.unlock();
    }

    MessageQueue *queue = it->second.get();

    map_lock_.unlock_shared();

    return queue;
}

void MessageQueueStore::Enqueue(MessageKey key, RealTimeMessage const &message) {
    MessageQueue *message_queue = FetchQueue(key);

    message_queue->lock.lock();
    message_queue->queue.push(message);
    message_queue->lock.unlock();

    sem_post(&message_queue->sem);
}

RealTimeMessage MessageQueueStore::BlockingDequeue(MessageKey key) {
    MessageQueue *message_queue = FetchQueue(key);

    sem_wait(&message_queue->sem);

    message_queue->lock.lock();
    RealTimeMessage result = message_queue->queue.back();
    message_queue->queue.pop();
    message_queue->lock.unlock();

    return result;
}

MessageQueueStore *MessageQueueStoreInstance() { return &gMessageQueueStore; }

} // namespace e8
