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

INCLUDEPATH += $$PWD/../../

SOURCES +=  \
    test_sign_message.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../common/unit_test_util
DEPENDPATH += $$PWD/../../common/unit_test_util

unix:!macx: LIBS += -L$$OUT_PWD/../ -lkeygen

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../common/time_util
DEPENDPATH += $$PWD/../../common/time_util

LIBS += -lboost_log
LIBS += -lboost_thread
LIBS += -pthread
