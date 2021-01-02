TEMPLATE = subdirs
SUBDIRS = \
    container/container.pro \
    flags/flags.pro \
    thread/thread.pro \
    time_util/time_util.pro \
    unit_test_util/unit_test_util.pro \
    _test_flags/_test_flags.pro \
    _test_time_util/_test_time_util.pro \
    _test_thread/_test_thread_pool \
    _test_container/_test_trie_map/_test_trie_map.pro \
    _test_container/_test_lru_hash_map/_test_lru_hash_map.pro \
    _test_container/_test_mutable_priority_queue

CONFIG += ordered
