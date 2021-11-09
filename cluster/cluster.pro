TEMPLATE = subdirs
SUBDIRS = \
    task_manager \
    placement \
    performer \
    conductor \
    task_manager/task_manager_main.pro \
    conductor/conductor_main.pro \
    _test_task_manager/_test_registry \
    _test_placement/_test_capability \
    _test_placement/_test_bucket \
    _test_placement/_test_hierarchy \
    _test_placement/_test_cluster_map \
    _test_conductor/_test_revision_work_pool \
    _test_conductor/_test_boardcast

CONFIG += ordered
