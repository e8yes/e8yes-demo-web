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
#include <memory>
#include <unordered_set>
#include <vector>

#include "common/thread/thread_pool.h"
#include "common/unit_test_util/unit_test_util.h"

class Operand : public e8::TaskStorageInterface {
  public:
    unsigned const operand;
    unsigned result = 0;

    Operand(unsigned operand) : operand(operand) {}
};

class AddOneTask : public e8::TaskInterface {
  public:
    void Run(e8::TaskStorageInterface *operand) const override {
        assert(operand != nullptr);
        static_cast<Operand *>(operand)->result = static_cast<Operand *>(operand)->operand + 1;
    }

    bool DropResourceOnCompletion() const override { return false; }
};

bool ScheduleTaskAndWaitForResultTest() {
    unsigned const kNumDataEntries = 1000;

    e8::ThreadPool thread_pool(/*num_workers=*/2);

    std::vector<std::unique_ptr<Operand>> operands;
    for (unsigned i = 0; i < kNumDataEntries; ++i) {
        operands.push_back(std::make_unique<Operand>(i));
    }

    std::shared_ptr<AddOneTask> task = std::make_shared<AddOneTask>();
    for (auto &operand : operands) {
        thread_pool.Schedule(task, std::move(operand));
    }

    std::vector<std::unique_ptr<e8::TaskStorageInterface>> results;
    for (auto &_ : operands) {
        (void)_;
        results.push_back(thread_pool.WaitForNextCompleted());
    }

    TEST_CONDITION(results.size() == kNumDataEntries);

    std::unordered_set<unsigned> operand_values;
    for (auto const &result : results) {
        Operand const *operand = static_cast<Operand const *>(result.get());
        operand_values.insert(operand->operand);

        TEST_CONDITION(operand->result == operand->operand + 1);
    }

    TEST_CONDITION(operand_values.size() == 1000);

    return true;
}

int main() {
    e8::BeginTestSuite("thread_pool");
    e8::RunTest("ScheduleTaskAndWaitForResultTest", ScheduleTaskAndWaitForResultTest);
    e8::EndTestSuite();
    return 0;
}
