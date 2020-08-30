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

#include <optional>
#include <string>

#include "common/container/lru_hash_map.h"
#include "common/unit_test_util/unit_test_util.h"

struct OnFetch {
    std::optional<int> operator()(std::string const &key) {
        if (key.length() != 1) {
            return std::nullopt;
        }

        fetch_freq[key]++;
        return key[0];
    }

    std::map<std::string, int> fetch_freq;
};

struct OnEvict {
    void operator()(int value) { evict_freq[value]++; }

    std::map<int, int> evict_freq;
};

bool FetchAndClearTest() {
    e8::LruHashMap<std::string, int, OnFetch, OnEvict> cache(/*max_size=*/2, OnFetch(), OnEvict());

    // Fetch items.
    // A, B, A, C, B
    // A and C will be fetched once whereas B will be fetched twice.
    // Boath A and B will be evicted once.
    std::optional<int> item_id = cache.Fetch("");
    TEST_CONDITION(!item_id.has_value());

    item_id = cache.Fetch("A");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'A');

    item_id = cache.Fetch("A");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'A');

    item_id = cache.Fetch("B");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'B');

    item_id = cache.Fetch("A");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'A');

    item_id = cache.Fetch("C");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'C');

    item_id = cache.Fetch("B");
    TEST_CONDITION(item_id.has_value());
    TEST_CONDITION(item_id.value() == 'B');

    // Verify the fetch frequencies.
    TEST_CONDITION(cache.FetchOperator().fetch_freq.at("A") == 1);
    TEST_CONDITION(cache.FetchOperator().fetch_freq.at("B") == 2);
    TEST_CONDITION(cache.FetchOperator().fetch_freq.at("C") == 1);

    // Verify the eviction frequencies.
    TEST_CONDITION(cache.EvictOperator().evict_freq.at('A') == 1);
    TEST_CONDITION(cache.EvictOperator().evict_freq.at('B') == 1);
    TEST_CONDITION(cache.EvictOperator().evict_freq.find('C') ==
                   cache.EvictOperator().evict_freq.end());

    cache.Clear();

    // Eventually, the number of evictions should be equal to the number of fetches.
    TEST_CONDITION(cache.EvictOperator().evict_freq.at('A') == 1);
    TEST_CONDITION(cache.EvictOperator().evict_freq.at('B') == 2);
    TEST_CONDITION(cache.EvictOperator().evict_freq.at('C') == 1);

    return true;
}

int main() {
    e8::BeginTestSuite("lru_hash_map");
    e8::RunTest("FetchAndClearTest", FetchAndClearTest);
    e8::EndTestSuite();
    return 0;
}
