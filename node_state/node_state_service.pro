CONFIG -= qt

TEMPLATE = lib
DEFINES += NODE_STATE_SERVICE_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../

SOURCES += \
    service/node_state_service.cc
HEADERS += \
    service/node_state_service.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../proto_cc
DEPENDPATH += $$PWD/../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../distributor/store/ -lnode_state_store

INCLUDEPATH += $$PWD/../distributor/store
DEPENDPATH += $$PWD/../distributor/store

unix:!macx: LIBS += -L$$OUT_PWD/../distributor/mutation_propagator/ -lmutation_propagator

INCLUDEPATH += $$PWD/../distributor/mutation_propagator
DEPENDPATH += $$PWD/../distributor/mutation_propagator

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
