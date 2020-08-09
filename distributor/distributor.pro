TEMPLATE = subdirs
SUBDIRS = \
    store/node_state_store.pro \
    node_state/node_state_service.pro \
    node_state/node_state_service_main.pro \
    distributor/distributor.pro \
    node_state_store_test

CONFIG += ordered
