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

#include <unordered_map>

#include "common/container/mutable_priority_queue.h"
#include "common/unit_test_util/unit_test_util.h"

bool MakeHeapAndPopTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.add(3);
    queue.add(-1);
    queue.add(4);
    queue.add(2);
    queue.add(3);
    queue.make_heap();

    TEST_CONDITION(queue.size() == 5);

    TEST_CONDITION(queue.top() == 4);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 2);
    queue.pop();
    TEST_CONDITION(queue.top() == -1);
    queue.pop();

    TEST_CONDITION(queue.empty());

    return true;
}

bool PushAndPopTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.push(3);
    queue.push(-1);
    queue.push(4);
    queue.push(2);
    queue.push(3);

    TEST_CONDITION(queue.size() == 5);

    TEST_CONDITION(queue.top() == 4);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 2);
    queue.pop();
    TEST_CONDITION(queue.top() == -1);
    queue.pop();

    TEST_CONDITION(queue.empty());

    return true;
}

bool PushAndReprioritizeTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.push(3);
    queue.push(-1);
    e8::MutablePriorityQueue<int>::iterator max_it = queue.push(4);
    queue.push(1);
    queue.push(3);

    *max_it = 2;
    queue.reprioritize(max_it);

    TEST_CONDITION(queue.size() == 5);

    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 2);
    queue.pop();
    TEST_CONDITION(queue.top() == 1);
    queue.pop();
    TEST_CONDITION(queue.top() == -1);
    queue.pop();

    TEST_CONDITION(queue.empty());

    return true;
}

bool PushAndErase1ElmentTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.push(3);
    queue.push(-1);
    e8::MutablePriorityQueue<int>::iterator it = queue.push(4);
    queue.push(1);
    queue.push(3);

    queue.erase(it);

    TEST_CONDITION(queue.size() == 4);

    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 3);
    queue.pop();
    TEST_CONDITION(queue.top() == 1);
    queue.pop();
    TEST_CONDITION(queue.top() == -1);
    queue.pop();

    TEST_CONDITION(queue.empty());

    return true;
}

bool PushAndEraseAllElement() {
    e8::MutablePriorityQueue<int> queue;
    e8::MutablePriorityQueue<int>::iterator it = queue.push(3);
    e8::MutablePriorityQueue<int>::iterator it2 = queue.push(-1);
    e8::MutablePriorityQueue<int>::iterator it3 = queue.push(4);
    e8::MutablePriorityQueue<int>::iterator it4 = queue.push(1);
    e8::MutablePriorityQueue<int>::iterator it5 = queue.push(3);

    queue.erase(it);
    queue.erase(it2);
    queue.erase(it3);
    queue.erase(it4);
    queue.erase(it5);

    TEST_CONDITION(queue.empty());

    return true;
}

bool PushAndIterateElementTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.push(3);
    queue.push(-1);
    queue.push(4);
    queue.push(1);
    queue.push(3);

    std::unordered_map<int, int> freq;
    for (auto entry : queue) {
        ++freq[entry];
    }

    TEST_CONDITION(freq.size() == 4);

    auto it = freq.find(-1);
    TEST_CONDITION(it != freq.end());
    TEST_CONDITION(it->second == 1);

    it = freq.find(1);
    TEST_CONDITION(it != freq.end());
    TEST_CONDITION(it->second == 1);

    it = freq.find(3);
    TEST_CONDITION(it != freq.end());
    TEST_CONDITION(it->second == 2);

    it = freq.find(4);
    TEST_CONDITION(it != freq.end());
    TEST_CONDITION(it->second == 1);

    return true;
}

bool EmptyQueueIteratorTest() {
    e8::MutablePriorityQueue<int> queue;
    TEST_CONDITION(queue.begin() == queue.end());

    return true;
}

bool FrontIteratorTest() {
    e8::MutablePriorityQueue<int> queue;
    queue.push(10);

    TEST_CONDITION(*queue.front() == 10);

    return true;
}

int main() {
    e8::BeginTestSuite("mutable_priority_queue");
    e8::RunTest("MakeHeapAndPopTest", MakeHeapAndPopTest);
    e8::RunTest("PushAndPopTest", PushAndPopTest);
    e8::RunTest("PushAndReprioritizeTest", PushAndReprioritizeTest);
    e8::RunTest("PushAndErase1ElmentTest", PushAndErase1ElmentTest);
    e8::RunTest("PushAndEraseAllElement", PushAndEraseAllElement);
    e8::RunTest("PushAndIterateElementTest", PushAndIterateElementTest);
    e8::RunTest("EmptyQueueIteratorTest", EmptyQueueIteratorTest);
    e8::RunTest("FrontIteratorTest", FrontIteratorTest);
    e8::EndTestSuite();
    return 0;
}
