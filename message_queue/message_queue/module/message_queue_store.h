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
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "proto_cc/real_time_message.pb.h"

namespace e8 {

using MessageKey = int64_t;

/**
 * @brief The MessageQueueStore class
 */
class MessageQueueStore {
  public:
    /**
     * @brief Enqueue
     * @param key
     * @param message
     */
    void Enqueue(MessageKey key, RealTimeMessage const &message);

    /**
     * @brief Dequeue
     * @param key
     * @return
     */
    std::optional<RealTimeMessage> Dequeue(MessageKey key);

  private:
    struct MessageQueue {
        std::queue<RealTimeMessage> queue;
        std::mutex queue_lock;
    };

    std::unordered_map<MessageKey, std::shared_ptr<MessageQueue>> queues_;
    std::shared_mutex map_lock_;
};

/**
 * @brief MessageQueueStoreInstance
 * @return
 */
MessageQueueStore *MessageQueueStoreInstance();

} // namespace e8

#endif // MESSAGE_QUEUE_STORE_H
