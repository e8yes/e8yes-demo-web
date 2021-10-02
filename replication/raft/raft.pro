CONFIG -= qt

TARGET = raft
TEMPLATE = lib

DEFINES += RAFT_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../


SOURCES += \
    background.cc \
    commit_pusher.cc \
    context.cc \
    election.cc \
    foreground.cc \
    integration_test/local_cluster.cc \
    integration_test/replication_monitor.cc \
    integration_test/role_monitor.cc \
    journal.cc \
    journal_replicator.cc \
    peer_set.cc \
    persister.cc \
    raft_instance.cc \
    role_at_term.cc \
    schedule.cc \
    service/raft_service.cc

HEADERS += \
    background.h \
    commit_pusher.h \
    common_types.h \
    context.h \
    election.h \
    foreground.h \
    integration_test/local_cluster.h \
    integration_test/replication_monitor.h \
    integration_test/role_monitor.h \
    journal.h \
    journal_replicator.h \
    peer_set.h \
    persister.h \
    raft_instance.h \
    role_at_term.h \
    schedule.h \
    service/raft_service.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
