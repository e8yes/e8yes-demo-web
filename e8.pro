TEMPLATE = subdirs
SUBDIRS = \
    common/common.pro \
    postgres/postgres.pro \
    keygen/keygen.pro \
    deployment_service/deployment.pro \
    demoweb_service/demoweb.pro

CONFIG += ordered
