TEMPLATE = app

qt -= core

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += $$PWD/../

SOURCES += \
    main.cc

unix:!macx: LIBS += -L$$OUT_PWD/../common/flags/ -lflags

INCLUDEPATH += $$PWD/../common/flags
DEPENDPATH += $$PWD/../common/flags

unix:!macx: LIBS += -L$$OUT_PWD/../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../proto_cc
DEPENDPATH += $$PWD/../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../distributor/store/ -lnode_state_store

INCLUDEPATH += $$PWD/../distributor/store
DEPENDPATH += $$PWD/../distributor/store

unix:!macx: LIBS += -L$$OUT_PWD/../distributor/mutation_propagator/ -lmutation_propagator

INCLUDEPATH += $$PWD/../distributor/mutation_propagator
DEPENDPATH += $$PWD/../distributor/mutation_propagator

unix:!macx: LIBS += -L$$OUT_PWD/./ -lnode_state_service

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
