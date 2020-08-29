TEMPLATE = subdirs
SUBDIRS = \
    store/node_state_store.pro \
    store_test/node_state_store_test \
    store_test/peer_store_test \
    distributor/distributor.pro \
    distributor_test \
    mutation_propagator/mutation_propagator.pro \
    mutation_propagator_test/sync_node_state_test

CONFIG += ordered
