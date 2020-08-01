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

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

#include "message_queue/message_queue/module/message_queue_store.h"

namespace e8 {
namespace {

static MessageQueueStore gMessageQueueStore;

} // namespace

void MessageQueueStore::Enqueue(MessageKey key, RealTimeMessage const &message) {
    map_lock_.lock_shared();

    auto it = queues_.find(key);
    if (it == queues_.end()) {
        map_lock_.lock();
        it = queues_.insert(std::make_pair(key, std::make_shared<MessageQueue>())).first;
        map_lock_.unlock();
    }

    MessageQueue *queue = it->second.get();
    queue->queue_lock.lock();
    queue->queue.push(message);
    queue->queue_lock.unlock();

    map_lock_.unlock_shared();
}

std::optional<RealTimeMessage> MessageQueueStore::Dequeue(MessageKey key) {
    std::optional<RealTimeMessage> result;

    map_lock_.lock_shared();

    auto it = queues_.find(key);
    if (it != queues_.end()) {
        it->second->queue_lock.lock();

        result = it->second->queue.back();
        it->second->queue.pop();

        it->second->queue_lock.unlock();
    }

    map_lock_.unlock_shared();

    return result;
}

MessageQueueStore *MessageQueueStoreInstance() { return &gMessageQueueStore; }

} // namespace e8
