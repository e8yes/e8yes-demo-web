CONFIG -= qt

TEMPLATE = lib
DEFINES += PROTO_CC_LIBRARY
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

HEADERS += \
    bucket.grpc.pb.h \
    bucket.pb.h \
    chat_message.grpc.pb.h \
    chat_message.pb.h \
    cluster_conductor.grpc.pb.h \
    cluster_conductor.pb.h \
    cluster.grpc.pb.h \
    cluster.pb.h \
    cluster_conductor_command.grpc.pb.h \
    cluster_conductor_command.pb.h \
    cluster_revision.grpc.pb.h \
    cluster_revision.pb.h \
    cluster_revision_command.grpc.pb.h \
    cluster_revision_command.pb.h \
    cluster_revision_work_pool.grpc.pb.h \
    cluster_revision_work_pool.pb.h \
    command.grpc.pb.h \
    command.pb.h \
    delta.grpc.pb.h \
    delta.pb.h \
    encryption_source.grpc.pb.h \
    encryption_source.pb.h \
    file.grpc.pb.h \
    file.pb.h \
    gomoku.grpc.pb.h \
    gomoku.pb.h \
    identity.grpc.pb.h \
    identity.pb.h \
    invitation_status.grpc.pb.h \
    invitation_status.pb.h \
    machine.grpc.pb.h \
    machine.pb.h \
    message_channel.grpc.pb.h \
    message_channel.pb.h \
    message_queue_stats.grpc.pb.h \
    message_queue_stats.pb.h \
    message_type.grpc.pb.h \
    message_type.pb.h \
    node.grpc.pb.h \
    node.pb.h \
    nullable_primitives.grpc.pb.h \
    nullable_primitives.pb.h \
    pagination.grpc.pb.h \
    pagination.pb.h \
    permission.grpc.pb.h \
    permission.pb.h \
    raft.grpc.pb.h \
    raft.pb.h \
    real_time_message.grpc.pb.h \
    real_time_message.pb.h \
    replication.grpc.pb.h \
    replication.pb.h \
    service_chat_message.grpc.pb.h \
    service_chat_message.pb.h \
    service_file.grpc.pb.h \
    service_file.pb.h \
    service_gomoku_classroom.grpc.pb.h \
    service_gomoku_classroom.pb.h \
    service_gomoku_game_instance.grpc.pb.h \
    service_gomoku_game_instance.pb.h \
    service_message_channel.grpc.pb.h \
    service_message_channel.pb.h \
    service_message_queue.grpc.pb.h \
    service_message_queue.pb.h \
    service_message_subscriber.grpc.pb.h \
    service_message_subscriber.pb.h \
    service_node_state.grpc.pb.h \
    service_node_state.pb.h \
    service_raft.grpc.pb.h \
    service_raft.pb.h \
    service_replication.grpc.pb.h \
    service_replication.pb.h \
    service_resource_worker.grpc.pb.h \
    service_resource_worker.pb.h \
    service_socialnetwork.grpc.pb.h \
    service_socialnetwork.pb.h \
    service_system.grpc.pb.h \
    service_system.pb.h \
    service_task_manager.grpc.pb.h \
    service_task_manager.pb.h \
    service_user.grpc.pb.h \
    service_user.pb.h \
    task.grpc.pb.h \
    task.pb.h \
    tensorflow/example.grpc.pb.h \
    tensorflow/example.pb.h \
    tensorflow/feature.grpc.pb.h \
    tensorflow/feature.pb.h \
    user_profile.grpc.pb.h \
    user_profile.pb.h \
    user_relation.grpc.pb.h \
    user_relation.pb.h

SOURCES += \
    bucket.grpc.pb.cc \
    bucket.pb.cc \
    chat_message.grpc.pb.cc \
    chat_message.pb.cc \
    cluster_conductor.grpc.pb.cc \
    cluster_conductor.pb.cc \
    cluster.grpc.pb.cc \
    cluster.pb.cc \
    cluster_conductor_command.grpc.pb.cc \
    cluster_conductor_command.pb.cc \
    cluster_revision.grpc.pb.cc \
    cluster_revision.pb.cc \
    cluster_revision_command.grpc.pb.cc \
    cluster_revision_command.pb.cc \
    cluster_revision_work_pool.grpc.pb.cc \
    cluster_revision_work_pool.pb.cc \
    command.grpc.pb.cc \
    command.pb.cc \
    delta.grpc.pb.cc \
    delta.pb.cc \
    encryption_source.grpc.pb.cc \
    encryption_source.pb.cc \
    file.grpc.pb.cc \
    file.pb.cc \
    gomoku.grpc.pb.cc \
    gomoku.pb.cc \
    identity.grpc.pb.cc \
    identity.pb.cc \
    invitation_status.grpc.pb.cc \
    invitation_status.pb.cc \
    machine.grpc.pb.cc \
    machine.pb.cc \
    message_channel.grpc.pb.cc \
    message_channel.pb.cc \
    message_queue_stats.grpc.pb.cc \
    message_queue_stats.pb.cc \
    message_type.grpc.pb.cc \
    message_type.pb.cc \
    node.grpc.pb.cc \
    node.pb.cc \
    nullable_primitives.grpc.pb.cc \
    nullable_primitives.pb.cc \
    pagination.grpc.pb.cc \
    pagination.pb.cc \
    permission.grpc.pb.cc \
    permission.pb.cc \
    raft.grpc.pb.cc \
    raft.pb.cc \
    real_time_message.grpc.pb.cc \
    real_time_message.pb.cc \
    replication.grpc.pb.cc \
    replication.pb.cc \
    service_chat_message.grpc.pb.cc \
    service_chat_message.pb.cc \
    service_file.grpc.pb.cc \
    service_file.pb.cc \
    service_gomoku_classroom.grpc.pb.cc \
    service_gomoku_classroom.pb.cc \
    service_gomoku_game_instance.grpc.pb.cc \
    service_gomoku_game_instance.pb.cc \
    service_message_channel.grpc.pb.cc \
    service_message_channel.pb.cc \
    service_message_queue.grpc.pb.cc \
    service_message_queue.pb.cc \
    service_message_subscriber.grpc.pb.cc \
    service_message_subscriber.pb.cc \
    service_node_state.grpc.pb.cc \
    service_node_state.pb.cc \
    service_raft.grpc.pb.cc \
    service_raft.pb.cc \
    service_replication.grpc.pb.cc \
    service_replication.pb.cc \
    service_resource_worker.grpc.pb.cc \
    service_resource_worker.pb.cc \
    service_socialnetwork.grpc.pb.cc \
    service_socialnetwork.pb.cc \
    service_system.grpc.pb.cc \
    service_system.pb.cc \
    service_task_manager.grpc.pb.cc \
    service_task_manager.pb.cc \
    service_user.grpc.pb.cc \
    service_user.pb.cc \
    task.grpc.pb.cc \
    task.pb.cc \
    tensorflow/example.grpc.pb.cc \
    tensorflow/example.pb.cc \
    tensorflow/feature.grpc.pb.cc \
    tensorflow/feature.pb.cc \
    user_profile.grpc.pb.cc \
    user_profile.pb.cc \
    user_relation.grpc.pb.cc \
    user_relation.pb.cc

LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
