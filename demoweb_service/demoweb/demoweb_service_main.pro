TEMPLATE = app

qt -= core

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

INCLUDEPATH += $$PWD/../../

SOURCES += \
    main.cc

unix:!macx: LIBS += -L$$OUT_PWD/./ -ldemoweb_service

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../../keygen
DEPENDPATH += $$PWD/../../keygen

unix:!macx: LIBS += -L$$OUT_PWD/../../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../../third_party/base64
DEPENDPATH += $$PWD/../../third_party/base64

unix:!macx: LIBS += -L$$OUT_PWD/../../common/flags/ -lflags

INCLUDEPATH += $$PWD/../../common/flags
DEPENDPATH += $$PWD/../../common/flags

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../../identity/ -lidentity

INCLUDEPATH += $$PWD/../../identity
DEPENDPATH += $$PWD/../../identity

unix:!macx: LIBS += -L$$OUT_PWD/../../distributor/store/ -lnode_state_store

INCLUDEPATH += $$PWD/../../distributor/store
DEPENDPATH += $$PWD/../../distributor/store

unix:!macx: LIBS += -L$$OUT_PWD/../../message_queue/subscriber/ -lsubscriber_service

INCLUDEPATH += $$PWD/../../message_queue/subscriber
DEPENDPATH += $$PWD/../../message_queue/subscriber

LIBS += -lpthread
LIBS += -ldl
LIBS += -lgrpc++ -lgrpc++_reflection
