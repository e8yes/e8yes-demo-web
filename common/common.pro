TEMPLATE = subdirs
SUBDIRS = \
    unit_test_util \
    container/container.pro \
    container_test/test_trie_map \
    container_test/test_lru_hash_map \
    flags \
    test_flags

CONFIG += ordered
