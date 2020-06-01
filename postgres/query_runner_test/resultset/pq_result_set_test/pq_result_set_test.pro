QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

TEMPLATE = app

INCLUDEPATH += $$PWD/../../../../

SOURCES +=  \
    tst_pq_result_set_test.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../../query_runner
DEPENDPATH += $$PWD/../../../query_runner

LIBS += -lpqxx
