TEMPLATE = subdirs
SUBDIRS = \
    store/node_state_store.pro \
    distributor/distributor.pro \
    mutation_propagator/mutation_propagator.pro \
    _test_store/_test_node_state_store/_test_node_state_store.pro \
    _test_store/_test_peer_store/_test_peer_store.pro \
    _test_distributor/_test_distributor.pro \
    _test_mutation_propagator/_test_sync_node_state/_test_sync_node_state.pro

CONFIG += ordered
