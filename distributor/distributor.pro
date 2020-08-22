TEMPLATE = subdirs
SUBDIRS = \
    distributor_test \
    store/node_state_store.pro \
    store_test/node_state_store_test \
    store_test/peer_store_test \
    node_state/node_state_service.pro \
    node_state/node_state_service_main.pro \
    distributor/distributor.pro

CONFIG += ordered
