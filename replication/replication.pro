TEMPLATE = subdirs
SUBDIRS = \
    raft \
    _test_raft/_test_schedule \
    _test_raft/_test_peer_set \
    _test_raft/_test_persister \
    _test_raft/_test_role_at_term \
    _test_raft/_test_journal \
    _test_raft/_test_context \
    _test_raft/_test_election \
    _test_raft/_test_agreement

CONFIG += ordered
