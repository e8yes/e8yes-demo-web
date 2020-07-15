QT       -= core gui
TARGET = demowebservice
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
    module/retrieve_contact.h \
    module/retrieve_user.h \
    module/system_user_group.h \
    module/user_identity.h \
    module/user_profile.h \
    proto_cc/chat_message.grpc.pb.h \
    proto_cc/chat_message.pb.h \
    proto_cc/encryption_source.grpc.pb.h \
    proto_cc/encryption_source.pb.h \
    proto_cc/file.grpc.pb.h \
    proto_cc/file.pb.h \
    proto_cc/identity.grpc.pb.h \
    proto_cc/identity.pb.h \
    proto_cc/invitation_status.grpc.pb.h \
    proto_cc/invitation_status.pb.h \
    proto_cc/message_channel.grpc.pb.h \
    proto_cc/message_channel.pb.h \
    proto_cc/message_type.grpc.pb.h \
    proto_cc/message_type.pb.h \
    proto_cc/nullable_primitives.grpc.pb.h \
    proto_cc/nullable_primitives.pb.h \
    proto_cc/pagination.grpc.pb.h \
    proto_cc/pagination.pb.h \
    proto_cc/permission.grpc.pb.h \
    proto_cc/permission.pb.h \
    proto_cc/service_chat_message.grpc.pb.h \
    proto_cc/service_chat_message.pb.h \
    proto_cc/service_file.grpc.pb.h \
    proto_cc/service_file.pb.h \
    proto_cc/service_message_channel.grpc.pb.h \
    proto_cc/service_message_channel.pb.h \
    proto_cc/service_socialnetwork.grpc.pb.h \
    proto_cc/service_socialnetwork.pb.h \
    proto_cc/service_system.grpc.pb.h \
    proto_cc/service_system.pb.h \
    proto_cc/service_user.grpc.pb.h \
    proto_cc/service_user.pb.h \
    proto_cc/user_profile.grpc.pb.h \
    proto_cc/user_profile.pb.h \
    proto_cc/user_relation.grpc.pb.h \
    proto_cc/user_relation.pb.h \
    service/file_service.h \
    service/service_util.h \
    service/social_network_service.h \
    service/user_service.h

SOURCES += \
    common_entity/contact_relation_entity.cc \
    common_entity/file_metadata_entity.cc \
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
    module/retrieve_contact.cc \
    module/retrieve_user.cc \
    module/user_identity.cc \
    module/user_profile.cc \
    proto_cc/chat_message.grpc.pb.cc \
    proto_cc/chat_message.pb.cc \
    proto_cc/encryption_source.grpc.pb.cc \
    proto_cc/encryption_source.pb.cc \
    proto_cc/file.grpc.pb.cc \
    proto_cc/file.pb.cc \
    proto_cc/identity.grpc.pb.cc \
    proto_cc/identity.pb.cc \
    proto_cc/invitation_status.grpc.pb.cc \
    proto_cc/invitation_status.pb.cc \
    proto_cc/message_channel.grpc.pb.cc \
    proto_cc/message_channel.pb.cc \
    proto_cc/message_type.grpc.pb.cc \
    proto_cc/message_type.pb.cc \
    proto_cc/nullable_primitives.grpc.pb.cc \
    proto_cc/nullable_primitives.pb.cc \
    proto_cc/pagination.grpc.pb.cc \
    proto_cc/pagination.pb.cc \
    proto_cc/permission.grpc.pb.cc \
    proto_cc/permission.pb.cc \
    proto_cc/service_chat_message.grpc.pb.cc \
    proto_cc/service_chat_message.pb.cc \
    proto_cc/service_file.grpc.pb.cc \
    proto_cc/service_file.pb.cc \
    proto_cc/service_message_channel.grpc.pb.cc \
    proto_cc/service_message_channel.pb.cc \
    proto_cc/service_socialnetwork.grpc.pb.cc \
    proto_cc/service_socialnetwork.pb.cc \
    proto_cc/service_system.grpc.pb.cc \
    proto_cc/service_system.pb.cc \
    proto_cc/service_user.grpc.pb.cc \
    proto_cc/service_user.pb.cc \
    proto_cc/user_profile.grpc.pb.cc \
    proto_cc/user_profile.pb.cc \
    proto_cc/user_relation.grpc.pb.cc \
    proto_cc/user_relation.pb.cc \
    service/file_service.cc \
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

LIBS += -lcrypt
LIBS += -lcrypto++
LIBS += -lpqxx
LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
