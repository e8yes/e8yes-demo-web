TEMPLATE = subdirs
SUBDIRS = \
    third_party/base64/base64.pro \
    third_party/uuid/uuid.pro \
    proto_cc/proto_cc.pro \
    common/common.pro \
    replication/replication.pro \
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
