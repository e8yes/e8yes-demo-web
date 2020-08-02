QT       -= core gui
TARGET = demoweb_service
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

HEADERS += \
    common_entity/contact_relation_entity.h \
    common_entity/file_metadata_entity.h \
    common_entity/message_channel_entity.h \
    common_entity/message_channel_has_user_entity.h \
    common_entity/user_entity.h \
    common_entity/user_group_entity.h \
    common_entity/user_group_has_file_entity.h \
    constant/context_key.h \
    constant/demoweb_database.h \
    constant/file_path.h \
    constant/pagination.h \
    environment/environment_context_interface.h \
    environment/host_id.h \
    environment/prod_environment_context.h \
    environment/test_environment_context.h \
    module/contact_invitation.h \
    module/create_user.h \
    module/file_access_validator.h \
    module/file_io.h \
    module/file_metadata.h \
    module/file_util.h \
    module/message_channel.h \
    module/retrieve_contact.h \
    module/retrieve_user.h \
    module/system_user_group.h \
    module/user_identity.h \
    module/user_profile.h \
    service/file_service.h \
    service/message_channel_service.h \
    service/service_util.h \
    service/social_network_service.h \
    service/user_service.h

SOURCES += \
    common_entity/contact_relation_entity.cc \
    common_entity/file_metadata_entity.cc \
    common_entity/message_channel_entity.cc \
    common_entity/message_channel_has_user_entity.cc \
    common_entity/user_entity.cc \
    common_entity/user_group_entity.cc \
    common_entity/user_group_has_file_entity.cc \
    environment/environment_context_interface.cc \
    environment/host_id.cc \
    environment/prod_environment_context.cc \
    environment/test_environment_context.cc \
    module/contact_invitation.cc \
    module/create_user.cc \
    module/file_access_validator.cc \
    module/file_io.cc \
    module/file_metadata.cc \
    module/file_util.cc \
    module/message_channel.cc \
    module/retrieve_contact.cc \
    module/retrieve_user.cc \
    module/user_identity.cc \
    module/user_profile.cc \
    service/file_service.cc \
    service/message_channel_service.cc \
    service/service_util.cc \
    service/social_network_service.cc \
    service/user_service.cc


unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../../keygen
DEPENDPATH += $$PWD/../../keygen

unix:!macx: LIBS += -L$$OUT_PWD/../../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../../third_party/base64
DEPENDPATH += $$PWD/../../third_party/base64

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

LIBS += -lcrypt
LIBS += -lcrypto++
LIBS += -lpqxx
LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
