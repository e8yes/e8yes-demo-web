TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -DBOOST_LOG_DYN_LINK
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

INCLUDEPATH += $$PWD/../../../../

SOURCES +=  \
    test_user_identity.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../../../common/unit_test_util
DEPENDPATH += $$PWD/../../../../common/unit_test_util

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../../../common/time_util
DEPENDPATH += $$PWD/../../../../common/time_util

unix:!macx: LIBS += -L$$OUT_PWD/../../../demoweb/ -ldemoweb_service

INCLUDEPATH += $$PWD/../../../demoweb
DEPENDPATH += $$PWD/../../../demoweb

unix:!macx: LIBS += -L$$OUT_PWD/../../../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../../../postgres/query_runner
DEPENDPATH += $$PWD/../../../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../../../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../../../../keygen
DEPENDPATH += $$PWD/../../../../keygen

unix:!macx: LIBS += -L$$OUT_PWD/../../../../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../../../../third_party/base64
DEPENDPATH += $$PWD/../../../../third_party/base64

LIBS += -lprotobuf

unix:!macx: LIBS += -L$$OUT_PWD/../../../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../../../proto_cc
DEPENDPATH += $$PWD/../../../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../../identity/ -lidentity

INCLUDEPATH += $$PWD/../../../../identity
DEPENDPATH += $$PWD/../../../../identity

unix:!macx: LIBS += -L$$OUT_PWD/../../../../distributor/store/ -lnode_state_store

INCLUDEPATH += $$PWD/../../../../distributor/store
DEPENDPATH += $$PWD/../../../../distributor/store

unix:!macx: LIBS += -L$$OUT_PWD/../../../../distributor/distributor/ -ldistributor

INCLUDEPATH += $$PWD/../../../../distributor/distributor
DEPENDPATH += $$PWD/../../../../distributor/distributor

unix:!macx: LIBS += -L$$OUT_PWD/../../../../message_queue/publisher/ -lpublisher

INCLUDEPATH += $$PWD/../../../../message_queue/publisher
DEPENDPATH += $$PWD/../../../../message_queue/publisher

unix:!macx: LIBS += -L$$OUT_PWD/../../../../message_queue/common/ -lmessage_queue_common

INCLUDEPATH += $$PWD/../../../../message_queue/common
DEPENDPATH += $$PWD/../../../../message_queue/common

LIBS += -lboost_log
LIBS += -lboost_thread
LIBS += -pthread
