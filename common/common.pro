TEMPLATE = subdirs
SUBDIRS = \
    unit_test_util \
    container/container.pro \
    flags/flags.pro \
    _test_flags/_test_flags.pro \
    _test_container/_test_trie_map/_test_trie_map.pro \
    _test_container/_test_lru_hash_map/_test_lru_hash_map.pro

CONFIG += ordered
