TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += $$PWD/../../../

SOURCES += \
    test_context.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../../common/unit_test_util
DEPENDPATH += $$PWD/../../../common/unit_test_util

unix:!macx: LIBS += -L$$OUT_PWD/../../raft/ -lraft

INCLUDEPATH += $$PWD/../../raft
DEPENDPATH += $$PWD/../../raft

unix:!macx: LIBS += -L$$OUT_PWD/../../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../../common/time_util
DEPENDPATH += $$PWD/../../../common/time_util

unix:!macx: LIBS += -L$$OUT_PWD/../../../common/thread/ -lthread

INCLUDEPATH += $$PWD/../../../common/thread
DEPENDPATH += $$PWD/../../../common/thread

unix:!macx: LIBS += -L$$OUT_PWD/../../../common/random/ -lrandom

INCLUDEPATH += $$PWD/../../../common/random
DEPENDPATH += $$PWD/../../../common/random

unix:!macx: LIBS += -L$$OUT_PWD/../../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../../proto_cc
DEPENDPATH += $$PWD/../../../proto_cc

LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
