CONFIG -= qt

TARGET = raft
TEMPLATE = lib

DEFINES += RAFT_MODULE_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../


SOURCES += \
    background.cc \
    context.cc \
    election.cc \
    foreground.cc \
    journal.cc \
    journal_replicator.cc \
    peer_set.cc \
    persister.cc \
    role_at_term.cc \
    schedule.cc \
    service/raft_service.cc

HEADERS += \
    background.h \
    common_types.h \
    context.h \
    election.h \
    foreground.h \
    journal.h \
    journal_replicator.h \
    peer_set.h \
    persister.h \
    role_at_term.h \
    schedule.h \
    service/raft_service.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
