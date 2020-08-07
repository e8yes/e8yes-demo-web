TEMPLATE = subdirs
SUBDIRS = \
    third_party/base64/base64.pro \
    proto_cc/proto_cc.pro \
    common/common.pro \
    distributor/distributor.pro \
    file_system/filesystemw.pro \
    postgres/postgres.pro \
    keygen/keygenw.pro \
    identity/identityw.pro \
    message_queue/message_queue.pro \
    demoweb_service/demoweb.pro

CONFIG += ordered
