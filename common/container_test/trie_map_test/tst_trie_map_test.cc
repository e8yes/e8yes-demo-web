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
#include <utility>

#include "common/container/trie_map.h"

class trie_map_test : public QObject {
    Q_OBJECT

  public:
    trie_map_test();
    ~trie_map_test();

  private slots:
    void test_insert();
    void test_insert_then_find();
    void test_insert_then_erase_then_find();
};

trie_map_test::trie_map_test() {}

trie_map_test::~trie_map_test() {}

void trie_map_test::test_insert() {
    e8::TrieMap<std::string, int> map;
    QVERIFY(map.size() == 0);
    QVERIFY(map.empty());

    std::pair<e8::TrieMap<std::string, int>::iterator, bool> insert_it =
        map.insert(std::make_pair("A", 1));
    QVERIFY(map.size() == 1);
    QVERIFY(insert_it.second == true);

    insert_it = map.insert(std::make_pair("ABC", 2));
    QVERIFY(map.size() == 2);
    QVERIFY(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AB", 3));
    QVERIFY(map.size() == 3);
    QVERIFY(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AC", 4));
    QVERIFY(map.size() == 4);
    QVERIFY(insert_it.second == true);

    insert_it = map.insert(std::make_pair("", 5));
    QVERIFY(map.size() == 5);
    QVERIFY(insert_it.second == true);

    insert_it = map.insert(std::make_pair("AB", 1000));
    QVERIFY(map.size() == 5);
    QVERIFY(insert_it.second == false);
}

void trie_map_test::test_insert_then_find() {
    e8::TrieMap<std::string, int> map;
    QVERIFY(map.size() == 0);
    QVERIFY(map.empty());

    std::pair<e8::TrieMap<std::string, int>::iterator, bool> insert_it =
        map.insert(std::make_pair("A", 1));
    e8::TrieMap<std::string, int>::iterator find_it = map.find("A");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "A");
    QVERIFY((*find_it).second == 1);

    insert_it = map.insert(std::make_pair("ABC", 2));
    find_it = map.find("ABC");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "ABC");
    QVERIFY((*find_it).second == 2);

    insert_it = map.insert(std::make_pair("AB", 3));
    find_it = map.find("AB");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "AB");
    QVERIFY((*find_it).second == 3);

    insert_it = map.insert(std::make_pair("AC", 4));
    find_it = map.find("AC");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "AC");
    QVERIFY((*find_it).second == 4);

    insert_it = map.insert(std::make_pair("", 5));
    find_it = map.find("");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "");
    QVERIFY((*find_it).second == 5);

    insert_it = map.insert(std::make_pair("AB", 1000));
    find_it = map.find("AB");
    QVERIFY(find_it == insert_it.first);
    QVERIFY((*find_it).first == "AB");
    QVERIFY((*find_it).second == 1000);
}

void trie_map_test::test_insert_then_erase_then_find() {
    e8::TrieMap<std::string, int> map;

    auto insert_it = map.insert(std::make_pair("A", 1));
    map.erase(insert_it.first);
    QVERIFY(map.size() == 0);
    QVERIFY(map.find("A") == map.end());

    map.insert(std::make_pair("ABCD", 2));
    map.insert(std::make_pair("AB", 3));
    QVERIFY(map.size() == 2);

    auto abcd_it = map.find("ABCD");
    map.erase(abcd_it);
    QVERIFY(map.size() == 1);
    QVERIFY(map.find("ABCD") == map.end());

    auto ab_it = map.find("AB");
    QVERIFY(ab_it != map.end());
    QVERIFY((*ab_it).first == "AB");
    QVERIFY((*ab_it).second == 3);
    map.erase(ab_it);
    QVERIFY(map.size() == 0);
    QVERIFY(map.find("AB") == map.end());
}

QTEST_APPLESS_MAIN(trie_map_test)

#include "tst_trie_map_test.moc"
