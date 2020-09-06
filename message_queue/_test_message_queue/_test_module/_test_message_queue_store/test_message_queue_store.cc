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

#include <thread>

#include "common/unit_test_util/unit_test_util.h"
#include "message_queue/message_queue/module/message_queue_store.h"
#include "proto_cc/real_time_message.pb.h"

bool StorageInstanceNotNullTest() {
    e8::MessageQueueStore *store_instance = e8::MessageQueueStoreInstance();
    TEST_CONDITION(store_instance != nullptr);

    return true;
}

bool EnqueueAndDequeueTest() {
    e8::MessageQueueStoreInstance()->Clear();

    e8::RealTimeMessage old_message;
    old_message.set_real_time_message_id(10);

    e8::RealTimeMessage new_message;
    new_message.set_real_time_message_id(11);

    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, old_message);
    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, new_message);

    e8::RealTimeMessage fetched_old_message;
    e8::MessageQueueStore::MessageQueue *queue =
        e8::MessageQueueStoreInstance()->BeginBlockingDequeue(/*key=*/1, /*wait_for_secs=*/-1,
                                                              &fetched_old_message);
    e8::MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/true);

    e8::RealTimeMessage fetched_new_message;
    queue = e8::MessageQueueStoreInstance()->BeginBlockingDequeue(/*key=*/1, /*wait_for_secs=*/-1,
                                                                  &fetched_new_message);
    e8::MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/true);

    TEST_CONDITION(old_message.real_time_message_id() ==
                   fetched_old_message.real_time_message_id());
    TEST_CONDITION(new_message.real_time_message_id() ==
                   fetched_new_message.real_time_message_id());

    return true;
}

void EnqueueInTheFuture() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    e8::RealTimeMessage message;
    message.set_real_time_message_id(10);

    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, message);
}

bool DequeueFutureMessageTest() {
    e8::MessageQueueStoreInstance()->Clear();

    std::thread thr(EnqueueInTheFuture);

    e8::RealTimeMessage future_message;
    e8::MessageQueueStore::MessageQueue *queue =
        e8::MessageQueueStoreInstance()->BeginBlockingDequeue(/*key=*/1, /*wait_for_secs=*/-1,
                                                              &future_message);
    e8::MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/true);
    TEST_CONDITION(future_message.real_time_message_id() == 10);

    thr.join();

    return true;
}

bool PeekOnlyTest() {
    e8::MessageQueueStoreInstance()->Clear();

    e8::RealTimeMessage old_message;
    old_message.set_real_time_message_id(10);

    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, old_message);

    e8::RealTimeMessage fetched_old_message;
    e8::MessageQueueStore::MessageQueue *queue =
        e8::MessageQueueStoreInstance()->BeginBlockingDequeue(/*key=*/1, /*wait_for_secs=*/-1,
                                                              &fetched_old_message);
    e8::MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/false);
    TEST_CONDITION(old_message.real_time_message_id() ==
                   fetched_old_message.real_time_message_id());

    queue = e8::MessageQueueStoreInstance()->BeginBlockingDequeue(/*key=*/1, /*wait_for_secs=*/-1,
                                                                  &fetched_old_message);
    e8::MessageQueueStoreInstance()->EndBlockingDequeue(queue, /*dequeue=*/true);
    TEST_CONDITION(old_message.real_time_message_id() ==
                   fetched_old_message.real_time_message_id());

    return true;
}

int main() {
    e8::BeginTestSuite("message_queue_store");
    e8::RunTest("StorageInstanceNotNullTest", StorageInstanceNotNullTest);
    e8::RunTest("EnqueueAndDequeueTest", EnqueueAndDequeueTest);
    e8::RunTest("DequeueFutureMessageTest", DequeueFutureMessageTest);
    e8::RunTest("PeekOnlyTest", PeekOnlyTest);
    e8::EndTestSuite();
    return 0;
}
