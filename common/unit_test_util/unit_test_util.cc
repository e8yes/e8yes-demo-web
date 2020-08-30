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

#include <cstdlib>
#include <iostream>

#include "common/unit_test_util/unit_test_util.h"

namespace e8 {
namespace {

static std::string gTestSuiteName = "NULL";
static bool gHasFailures = false;

void BeginTest(std::string const &test_name) {
    std::cout << "\x1B[33m" << gTestSuiteName << ":" << test_name << " -> Running\033[0m"
              << std::endl;
}

void EndTest(std::string const &test_name, bool success) {
    if (success) {
        std::cout << "\x1B[32m" << gTestSuiteName << ":" << test_name << " -> Pass"
                  << "\033[0m" << std::endl;
    } else {
        std::cout << "\x1B[31m" << gTestSuiteName << ":" << test_name << " -> Failed"
                  << "\033[0m" << std::endl;
    }
}

} // namespace

void BeginTestSuite(std::string const &test_suite_name) {
    std::cout << "\x1B[34m"
              << "============================================"
              << "\033[0m" << std::endl;
    std::cout << "\x1B[34m"
              << "******** Start testing of " << test_suite_name << " *********"
              << "\033[0m" << std::endl;

    gTestSuiteName = test_suite_name;
}

void RunTest(std::string const &test_name, TestFn test_fn) {
    BeginTest(test_name);
    if (test_fn()) {
        EndTest(test_name, /*success=*/true);
    } else {
        gHasFailures = true;
        EndTest(test_name, /*success=*/false);
    }
}

void EndTestSuite() {
    if (!gHasFailures) {
        std::cout << "\x1B[34m"
                  << "********* Finished testing of " << gTestSuiteName << " *********"
                  << "\033[0m" << std::endl;
    } else {
        std::cout << "\x1B[31m"
                  << "********* Failing " << gTestSuiteName << " *********"
                  << "\033[0m" << std::endl;
        std::abort();
    }

    std::cout << "\x1B[34m"
              << "============================================"
              << "\033[0m" << std::endl;
}

} // namespace e8
