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

#include <string>
#include <utility>

#include "common/container/trie_map.h"
#include "common/unit_test_util/unit_test_util.h"

bool InsertTest() {
    e8::TrieMap<std::string, int> map;
    TEST_CONDITION(map.size() == 0);
    TEST_CONDITION(map.empty());

    std::pair<e8::TrieMap<std::string, int>::iterator, bool> insert_it =
        map.insert(std::make_pair("A", 1));
    TEST_CONDITION(map.size() == 1);
    TEST_CONDITION(insert_it.second == true);

    insert_it = map.insert(std::make_pair("ABC", 2));
    TEST_CONDITION(map.size() == 2);
    TEST_CONDITION(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AB", 3));
    TEST_CONDITION(map.size() == 3);
    TEST_CONDITION(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AC", 4));
    TEST_CONDITION(map.size() == 4);
    TEST_CONDITION(insert_it.second == true);

    insert_it = map.insert(std::make_pair("", 5));
    TEST_CONDITION(map.size() == 5);
    TEST_CONDITION(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AB", 1000));
    TEST_CONDITION(map.size() == 5);
    TEST_CONDITION(insert_it.second == false);

    return true;
}

bool InsertThenFindTest() {
    e8::TrieMap<std::string, int> map;
    TEST_CONDITION(map.size() == 0);
    TEST_CONDITION(map.empty());

    std::pair<e8::TrieMap<std::string, int>::iterator, bool> insert_it =
        map.insert(std::make_pair("A", 1));
    e8::TrieMap<std::string, int>::iterator find_it = map.find("A");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "A");
    TEST_CONDITION((*find_it).second == 1);

    insert_it = map.insert(std::make_pair("ABC", 2));
    find_it = map.find("ABC");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "ABC");
    TEST_CONDITION((*find_it).second == 2);

    insert_it = map.insert(std::make_pair("AB", 3));
    find_it = map.find("AB");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "AB");
    TEST_CONDITION((*find_it).second == 3);

    insert_it = map.insert(std::make_pair("AC", 4));
    find_it = map.find("AC");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "AC");
    TEST_CONDITION((*find_it).second == 4);

    insert_it = map.insert(std::make_pair("", 5));
    find_it = map.find("");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "");
    TEST_CONDITION((*find_it).second == 5);

    insert_it = map.insert(std::make_pair("AB", 1000));
    find_it = map.find("AB");
    TEST_CONDITION(find_it == insert_it.first);
    TEST_CONDITION((*find_it).first == "AB");
    TEST_CONDITION((*find_it).second == 1000);

    return true;
}

bool InsertThenEraseThenFindTest() {
    e8::TrieMap<std::string, int> map;

    auto insert_it = map.insert(std::make_pair("A", 1));
    map.erase(insert_it.first);
    TEST_CONDITION(map.size() == 0);
    TEST_CONDITION(map.find("A") == map.end());

    map.insert(std::make_pair("ABCD", 2));
    map.insert(std::make_pair("AB", 3));
    TEST_CONDITION(map.size() == 2);

    auto abcd_it = map.find("ABCD");
    map.erase(abcd_it);
    TEST_CONDITION(map.size() == 1);
    TEST_CONDITION(map.find("ABCD") == map.end());

    auto ab_it = map.find("AB");
    TEST_CONDITION(ab_it != map.end());
    TEST_CONDITION((*ab_it).first == "AB");
    TEST_CONDITION((*ab_it).second == 3);
    map.erase(ab_it);
    TEST_CONDITION(map.size() == 0);
    TEST_CONDITION(map.find("AB") == map.end());

    return true;
}

int main() {
    e8::BeginTestSuite("test_trie_map");
    e8::RunTest("InsertTest", InsertTest);
    e8::RunTest("InsertThenFindTest", InsertThenFindTest);
    e8::RunTest("InsertThenEraseThenFindTest", InsertThenEraseThenFindTest);
    e8::EndTestSuite();
    return 0;
}
