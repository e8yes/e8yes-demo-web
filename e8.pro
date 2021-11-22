TEMPLATE = subdirs
SUBDIRS = \
    third_party \
    proto_cc \
    common \
    common/commonb.pro \
    replication \
    replication/replicationb.pro \
    cluster \
    cluster/clusterb.pro \
    distributor \
    postgres \
    postgres/postgresb.pro \
    keygen \
    identity \
    file_system/filesystemw.pro \
    message_queue \
    message_queue/message_queueb.pro \
    node_state/node_statew.pro \
    # gomoku \
    demoweb_service/demoweb.pro \
    demoweb_service/demowebb.pro

CONFIG += ordered
