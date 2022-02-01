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

#include <boost/log/trivial.hpp>
#include <cstdlib>

#include "common/unit_test_util/unit_test_util.h"

namespace e8 {
namespace {

static std::string gTestSuiteName = "NULL";
static bool gHasFailures = false;

void BeginTest(std::string const &test_name) {
    BOOST_LOG_TRIVIAL(info) << "\x1B[33m" << gTestSuiteName << ":" << test_name
                            << " -> Running\033[0m";
}

void EndTest(std::string const &test_name, bool success) {
    if (success) {
        BOOST_LOG_TRIVIAL(info) << "\x1B[32m" << gTestSuiteName << ":" << test_name << " -> Pass"
                                << "\033[0m";
    } else {
        BOOST_LOG_TRIVIAL(info) << "\x1B[31m" << gTestSuiteName << ":" << test_name << " -> Failed"
                                << "\033[0m";
    }
}

} // namespace

void BeginTestSuite(std::string const &test_suite_name) {
    BOOST_LOG_TRIVIAL(info) << "\x1B[34m"
                            << "============================================"
                            << "\033[0m";
    BOOST_LOG_TRIVIAL(info) << "\x1B[34m"
                            << "******** Start testing of " << test_suite_name << " *********"
                            << "\033[0m";

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
        BOOST_LOG_TRIVIAL(info) << "\x1B[34m"
                                << "********* Finished testing of " << gTestSuiteName
                                << " *********"
                                << "\033[0m";
    } else {
        BOOST_LOG_TRIVIAL(fatal) << "\x1B[31m"
                                 << "********* Failing " << gTestSuiteName << " *********"
                                 << "\033[0m";
        std::abort();
    }

    BOOST_LOG_TRIVIAL(info) << "\x1B[34m"
                            << "============================================"
                            << "\033[0m";
}

} // namespace e8
