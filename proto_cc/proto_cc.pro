CONFIG -= qt

TEMPLATE = lib
DEFINES += IDENTITY_LIBRARY
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection

HEADERS += \
    chat_message.grpc.pb.h \
    chat_message.pb.h \
    encryption_source.grpc.pb.h \
    encryption_source.pb.h \
    file.grpc.pb.h \
    file.pb.h \
    identity.grpc.pb.h \
    identity.pb.h \
    invitation_status.grpc.pb.h \
    invitation_status.pb.h \
    message_channel.grpc.pb.h \
    message_channel.pb.h \
    message_type.grpc.pb.h \
    message_type.pb.h \
    nullable_primitives.grpc.pb.h \
    nullable_primitives.pb.h \
    pagination.grpc.pb.h \
    pagination.pb.h \
    permission.grpc.pb.h \
    permission.pb.h \
    real_time_message.grpc.pb.h \
    real_time_message.pb.h \
    service_chat_message.grpc.pb.h \
    service_chat_message.pb.h \
    service_file.grpc.pb.h \
    service_file.pb.h \
    service_message_channel.grpc.pb.h \
    service_message_channel.pb.h \
    service_message_queue.grpc.pb.h \
    service_message_queue.pb.h \
    service_message_subscriber.grpc.pb.h \
    service_message_subscriber.pb.h \
    service_socialnetwork.grpc.pb.h \
    service_socialnetwork.pb.h \
    service_system.grpc.pb.h \
    service_system.pb.h \
    service_user.grpc.pb.h \
    service_user.pb.h \
    user_profile.grpc.pb.h \
    user_profile.pb.h \
    user_relation.grpc.pb.h \
    user_relation.pb.h

SOURCES += \
    chat_message.grpc.pb.cc \
    chat_message.pb.cc \
    encryption_source.grpc.pb.cc \
    encryption_source.pb.cc \
    file.grpc.pb.cc \
    file.pb.cc \
    identity.grpc.pb.cc \
    identity.pb.cc \
    invitation_status.grpc.pb.cc \
    invitation_status.pb.cc \
    message_channel.grpc.pb.cc \
    message_channel.pb.cc \
    message_type.grpc.pb.cc \
    message_type.pb.cc \
    nullable_primitives.grpc.pb.cc \
    nullable_primitives.pb.cc \
    pagination.grpc.pb.cc \
    pagination.pb.cc \
    permission.grpc.pb.cc \
    permission.pb.cc \
    real_time_message.grpc.pb.cc \
    real_time_message.pb.cc \
    service_chat_message.grpc.pb.cc \
    service_chat_message.pb.cc \
    service_file.grpc.pb.cc \
    service_file.pb.cc \
    service_message_channel.grpc.pb.cc \
    service_message_channel.pb.cc \
    service_message_queue.grpc.pb.cc \
    service_message_queue.pb.cc \
    service_message_subscriber.grpc.pb.cc \
    service_message_subscriber.pb.cc \
    service_socialnetwork.grpc.pb.cc \
    service_socialnetwork.pb.cc \
    service_system.grpc.pb.cc \
    service_system.pb.cc \
    service_user.grpc.pb.cc \
    service_user.pb.cc \
    user_profile.grpc.pb.cc \
    user_profile.pb.cc \
    user_relation.grpc.pb.cc \
    user_relation.pb.cc
