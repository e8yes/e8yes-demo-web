#include <QtTest>

#include "util/lru_hash_map.h"

class lru_hash_map_test : public QObject {
    Q_OBJECT

  public:
    lru_hash_map_test();
    ~lru_hash_map_test();

  private slots:
    void test_fetch_and_clear();
};

lru_hash_map_test::lru_hash_map_test() {}

lru_hash_map_test::~lru_hash_map_test() {}

struct OnFetch {
    int operator()(std::string const &key) {
        fetch_freq[key]++;
        return key[0];
    }
    std::map<std::string, int> fetch_freq;
};

struct OnEvict {
    void operator()(int value) { evict_freq[value]++; }
    std::map<int, int> evict_freq;
};

void lru_hash_map_test::test_fetch_and_clear() {
    e8::LruHashMap<std::string, int, OnFetch, OnEvict> cache(/*max_size=*/2, OnFetch(), OnEvict());

    // Fetch items.
    // A, B, A, C, B
    // A and C will be fetched once whereas B will be fetched twice.
    // Boath A and B will be evicted once.
    int item_id = cache.Fetch("A");
    QVERIFY(item_id == 'A');
    item_id = cache.Fetch("A");
    QVERIFY(item_id == 'A');
    item_id = cache.Fetch("B");
    QVERIFY(item_id == 'B');
    item_id = cache.Fetch("A");
    QVERIFY(item_id == 'A');
    item_id = cache.Fetch("C");
    QVERIFY(item_id == 'C');
    item_id = cache.Fetch("B");
    QVERIFY(item_id == 'B');

    // Verify the fetch frequencies.
    QVERIFY(cache.FetchOperator().fetch_freq.at("A") == 1);
    QVERIFY(cache.FetchOperator().fetch_freq.at("B") == 2);
    QVERIFY(cache.FetchOperator().fetch_freq.at("C") == 1);

    // Verify the eviction frequencies.
    QVERIFY(cache.EvictOperator().evict_freq.at('A') == 1);
    QVERIFY(cache.EvictOperator().evict_freq.at('B') == 1);
    QVERIFY(cache.EvictOperator().evict_freq.find('C') == cache.EvictOperator().evict_freq.end());

    cache.Clear();

    // Eventually, the number of evictions should be equal to the number of fetches.
    QVERIFY(cache.EvictOperator().evict_freq.at('A') == 1);
    QVERIFY(cache.EvictOperator().evict_freq.at('B') == 2);
    QVERIFY(cache.EvictOperator().evict_freq.at('C') == 1);
}

QTEST_APPLESS_MAIN(lru_hash_map_test)

#include "tst_lru_hash_map_test.moc"
