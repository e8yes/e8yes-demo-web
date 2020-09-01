CONFIG -= qt

TEMPLATE = lib
DEFINES += MESSAGE_QUEUE_COMMON_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += ../../

SOURCES += \
    message_queue_distributor.cc
HEADERS += \
    entity.h \
    message_queue_distributor.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../../distributor/distributor/ -ldistributor

INCLUDEPATH += $$PWD/../../distributor/distributor
DEPENDPATH += $$PWD/../../distributor/distributor

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
