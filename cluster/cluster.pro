TEMPLATE = subdirs
SUBDIRS = \
    placement \
    conductor \
    conductor/conductor_main.pro \
    _test_placement/_test_capability \
    _test_placement/_test_bucket \
    _test_placement/_test_hierarchy \
    _test_placement/_test_cluster_map

CONFIG += ordered
