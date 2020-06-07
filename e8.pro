TEMPLATE = subdirs
SUBDIRS = \
    third_party/base64/base64.pro \
    common/common.pro \
    file_system/filesystemw.pro \
    postgres/postgres.pro \
    keygen/keygenw.pro \
    demoweb_service/demoweb.pro

CONFIG += ordered
