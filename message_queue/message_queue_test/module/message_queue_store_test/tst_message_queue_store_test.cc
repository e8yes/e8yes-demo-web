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

#include <QtTest>
#include <thread>

#include "message_queue/message_queue/module/message_queue_store.h"
#include "proto_cc/real_time_message.pb.h"

class message_queue_store_test : public QObject {
    Q_OBJECT

  public:
    message_queue_store_test();
    ~message_queue_store_test();

  private slots:
    void enqueue_and_dequeue_test();
    void dequeue_future_message_test();
};

message_queue_store_test::message_queue_store_test() {}

message_queue_store_test::~message_queue_store_test() {}

void message_queue_store_test::enqueue_and_dequeue_test() {
    e8::MessageQueueStoreInstance()->Clear();

    e8::RealTimeMessage old_message;
    old_message.set_real_time_message_id(10);

    e8::RealTimeMessage new_message;
    new_message.set_real_time_message_id(11);

    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, old_message);
    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, new_message);

    e8::RealTimeMessage fetched_old_message =
        e8::MessageQueueStoreInstance()->BlockingDequeue(/*key=*/1);
    e8::RealTimeMessage fetched_new_message =
        e8::MessageQueueStoreInstance()->BlockingDequeue(/*key=*/1);

    QVERIFY(old_message.real_time_message_id() == fetched_old_message.real_time_message_id());
    QVERIFY(new_message.real_time_message_id() == fetched_new_message.real_time_message_id());
}

void EnqueueInTheFuture() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    e8::RealTimeMessage message;
    message.set_real_time_message_id(10);

    e8::MessageQueueStoreInstance()->Enqueue(/*key=*/1, message);
}

void message_queue_store_test::dequeue_future_message_test() {
    e8::MessageQueueStoreInstance()->Clear();

    std::thread thr(EnqueueInTheFuture);

    e8::RealTimeMessage future_message =
        e8::MessageQueueStoreInstance()->BlockingDequeue(/*key=*/1);
    QVERIFY(future_message.real_time_message_id() == 10);

    thr.join();
}

QTEST_APPLESS_MAIN(message_queue_store_test)

#include "tst_message_queue_store_test.moc"
