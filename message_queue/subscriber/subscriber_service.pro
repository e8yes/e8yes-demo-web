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
    environment/environment_context_interface.cc \
    environment/prod_environment_context.cc \
    environment/test_environment_context.cc \
    service/message_subscriber_service.cc
HEADERS += \
    environment/environment_context_interface.h \
    environment/prod_environment_context.h \
    environment/test_environment_context.h \
    service/message_subscriber_service.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../../distributor/store/ -lnode_state_store

INCLUDEPATH += $$PWD/../../distributor/store
DEPENDPATH += $$PWD/../../distributor/store

unix:!macx: LIBS += -L$$OUT_PWD/../../distributor/distributor/ -ldistributor

INCLUDEPATH += $$PWD/../../distributor/distributor
DEPENDPATH += $$PWD/../../distributor/distributor

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
