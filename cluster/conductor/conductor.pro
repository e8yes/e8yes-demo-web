CONFIG -= qt

TARGET = cluster_conductor
TEMPLATE = lib

DEFINES += CLUSTER_REVISION_LIBRARY

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
    boardcast.cc \
    client.cc \
    conductor.cc \
    store.cc


HEADERS += \
    background.h \
    boardcast.h \
    client.h \
    condutor.h \
    store.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
