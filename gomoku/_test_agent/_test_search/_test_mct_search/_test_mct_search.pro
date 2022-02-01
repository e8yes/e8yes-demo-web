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

INCLUDEPATH += $$PWD/../../../../

SOURCES += \
    test_mct_search.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../agent/ -lgomoku_agent

INCLUDEPATH += $$PWD/../../../agent
DEPENDPATH += $$PWD/../../../agent

unix:!macx: LIBS += -L$$OUT_PWD/../../../game/ -lgomoku_game

INCLUDEPATH += $$PWD/../../../game
DEPENDPATH += $$PWD/../../../game

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../../../common/unit_test_util
DEPENDPATH += $$PWD/../../../../common/unit_test_util

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/thread/ -lthread

INCLUDEPATH += $$PWD/../../../../common/thread
DEPENDPATH += $$PWD/../../../../common/thread

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/random/ -lrandom

INCLUDEPATH += $$PWD/../../../../common/random
DEPENDPATH += $$PWD/../../../../common/random

unix:!macx: LIBS += -L$$OUT_PWD/../../../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../../../common/time_util
DEPENDPATH += $$PWD/../../../../common/time_util

LIBS += -lboost_log
LIBS += -lboost_thread
LIBS += -pthread
LIBS += -ltensorflow
LIBS += -ltensorflow_framework
LIBS += -ltensorflowlite_c
