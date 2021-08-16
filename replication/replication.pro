TEMPLATE = subdirs
SUBDIRS = \
    raft \
    _test_raft/_test_schedule \
    _test_raft/_test_persister \
    _test_raft/_test_role_at_term \
    _test_raft/_test_context

CONFIG += ordered
