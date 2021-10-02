TEMPLATE = subdirs
SUBDIRS = \
    third_party \
    proto_cc/proto_cc.pro \
    common/common.pro \
    replication/replication.pro \
    cluster/cluster.pro \
    distributor/distributor.pro \
    postgres/postgres.pro \
    keygen/keygenw.pro \
    identity/identityw.pro \
    file_system/filesystemw.pro \
    message_queue/message_queue.pro \
    node_state/node_statew.pro \
    gomoku/gomoku.pro \
    demoweb_service/demoweb.pro

CONFIG += ordered
