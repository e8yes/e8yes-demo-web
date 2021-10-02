CONFIG -= qt

TARGET = raft_runner
TEMPLATE = lib

DEFINES += RAFT_RUNNER_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    client.cc \
    command_queue.cc \
    fulfillment_pool.cc \
    future.cc \
    integration_test/key_value_store.cc \
    integration_test/local_cluster.cc \
    run_event_id.cc \
    runner.cc \
    service/replication_service.cc

HEADERS += \
    client.h \
    command_queue.h \
    fulfillment_pool.h \
    future.h \
    integration_test/key_value_store.h \
    integration_test/local_cluster.h \
    run_event_id.h \
    runner.h \
    service/replication_service.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
