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

#ifndef UNIT_TEST_UTIL_H
#define UNIT_TEST_UTIL_H

#include <boost/log/trivial.hpp>
#include <string>

namespace e8 {

/**
 * @brief BeginTestSuite
 */
void BeginTestSuite(std::string const &test_suite_name);

typedef bool (*TestFn)();

/**
 * @brief RunTest
 */
void RunTest(std::string const &test_name, TestFn test_fn);

/**
 * @brief EndTestSuite
 */
void EndTestSuite();

#define TEST_CONDITION(__condition__)                                                              \
    if (!(__condition__)) {                                                                        \
        BOOST_LOG_TRIVIAL(error) << "Failing condition in file=" << __FILE__                       \
                                 << " at line=" << __LINE__;                                       \
        return false;                                                                              \
    }

} // namespace e8

#endif // UNIT_TEST_UTIL_H
