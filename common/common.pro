TEMPLATE = subdirs
SUBDIRS = \
    container/container.pro \
    container_test/trie_map_test \
    container_test/lru_hash_map_test \
    flags \
    flags_test

CONFIG += ordered
