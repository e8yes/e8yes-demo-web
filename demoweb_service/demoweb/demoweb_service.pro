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
    common_entity/chat_message_entity.h \
    common_entity/chat_message_group_entity.h \
    common_entity/contact_relation_entity.h \
    common_entity/file_metadata_entity.h \
    common_entity/message_channel_entity.h \
    common_entity/message_channel_has_user_entity.h \
    common_entity/user_entity.h \
    common_entity/user_group_entity.h \
    common_entity/user_group_has_file_entity.h \
    constant/demoweb_database.h \
    constant/file_path.h \
    constant/pagination.h \
    environment/environment_context_interface.h \
    environment/host_id.h \
    environment/prod_environment_context.h \
    environment/test_environment_context.h \
    module/baseline_user.h \
    module/chat_message.h \
    module/chat_message_group.h \
    module/chat_message_storage.h \
    module/contact_invitation.h \
    module/contact_storage.h \
    module/file_access_validator.h \
    module/file_io.h \
    module/file_metadata.h \
    module/file_util.h \
    module/message_channel.h \
    module/message_channel_storage.h \
    module/push_message.h \
    module/search_user.h \
    module/system_user_group.h \
    module/user_identity.h \
    module/user_profile.h \
    module/user_storage.h \
    pbac/message_channel_attributes.h \
    pbac/message_channel_member_attributes.h \
    pbac/message_channel_pbac.h \
    service/file_service.h \
    service/message_channel_service.h \
    service/service_util.h \
    service/social_network_service.h \
    service/user_service.h

SOURCES += \
    common_entity/chat_message_entity.cc \
    common_entity/chat_message_group_entity.cc \
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
    module/baseline_user.cc \
    module/chat_message.cc \
    module/chat_message_group.cc \
    module/chat_message_storage.cc \
    module/contact_invitation.cc \
    module/contact_storage.cc \
    module/file_access_validator.cc \
    module/file_io.cc \
    module/file_metadata.cc \
    module/file_util.cc \
    module/message_channel.cc \
    module/message_channel_storage.cc \
    module/push_message.cc \
    module/search_user.cc \
    module/user_identity.cc \
    module/user_profile.cc \
    module/user_storage.cc \
    pbac/message_channel_attributes.cc \
    pbac/message_channel_member_attributes.cc \
    pbac/message_channel_pbac.cc \
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

unix:!macx: LIBS += -L$$OUT_PWD/../../identity/ -lidentity

INCLUDEPATH += $$PWD/../../identity
DEPENDPATH += $$PWD/../../identity

unix:!macx: LIBS += -L$$OUT_PWD/../../message_queue/publisher/ -lpublisher

INCLUDEPATH += $$PWD/../../message_queue/publisher
DEPENDPATH += $$PWD/../../message_queue/publisher

LIBS += -lcrypt
LIBS += -lcrypto++
LIBS += -lpqxx
LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
