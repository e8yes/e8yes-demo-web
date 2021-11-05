CONFIG -= qt

TARGET = cluster_task_manager
TEMPLATE = lib

DEFINES += CLUSTER_TASK_MANAGER_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    context.cc \
    crud.cc \
    history.cc \
    pid_slots.cc \
    service.cc \
    startup.cc
HEADERS += \
    common_types.h \
    context.h \
    crud.h \
    history.h \
    pid_slots.h \
    service.h \
    startup.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
