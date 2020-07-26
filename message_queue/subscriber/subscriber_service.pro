CONFIG -= qt

TEMPLATE = lib
DEFINES += SUBSCRIBER_SERVICE_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
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
    proto_cc/real_time_message.grpc.pb.cc \
    proto_cc/real_time_message.pb.cc \
    proto_cc/service_chat_message.grpc.pb.cc \
    proto_cc/service_chat_message.pb.cc \
    proto_cc/service_file.grpc.pb.cc \
    proto_cc/service_file.pb.cc \
    proto_cc/service_message_channel.grpc.pb.cc \
    proto_cc/service_message_channel.pb.cc \
    proto_cc/service_message_queue.grpc.pb.cc \
    proto_cc/service_message_queue.pb.cc \
    proto_cc/service_message_subscriber.grpc.pb.cc \
    proto_cc/service_message_subscriber.pb.cc \
    proto_cc/service_socialnetwork.grpc.pb.cc \
    proto_cc/service_socialnetwork.pb.cc \
    proto_cc/service_system.grpc.pb.cc \
    proto_cc/service_system.pb.cc \
    proto_cc/service_user.grpc.pb.cc \
    proto_cc/service_user.pb.cc \
    proto_cc/user_profile.grpc.pb.cc \
    proto_cc/user_profile.pb.cc \
    proto_cc/user_relation.grpc.pb.cc \
    proto_cc/user_relation.pb.cc

HEADERS += \
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
    proto_cc/real_time_message.grpc.pb.h \
    proto_cc/real_time_message.pb.h \
    proto_cc/service_chat_message.grpc.pb.h \
    proto_cc/service_chat_message.pb.h \
    proto_cc/service_file.grpc.pb.h \
    proto_cc/service_file.pb.h \
    proto_cc/service_message_channel.grpc.pb.h \
    proto_cc/service_message_channel.pb.h \
    proto_cc/service_message_queue.grpc.pb.h \
    proto_cc/service_message_queue.pb.h \
    proto_cc/service_message_subscriber.grpc.pb.h \
    proto_cc/service_message_subscriber.pb.h \
    proto_cc/service_socialnetwork.grpc.pb.h \
    proto_cc/service_socialnetwork.pb.h \
    proto_cc/service_system.grpc.pb.h \
    proto_cc/service_system.pb.h \
    proto_cc/service_user.grpc.pb.h \
    proto_cc/service_user.pb.h \
    proto_cc/user_profile.grpc.pb.h \
    proto_cc/user_profile.pb.h \
    proto_cc/user_relation.grpc.pb.h \
    proto_cc/user_relation.pb.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
