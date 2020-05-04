QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

TEMPLATE = app

INCLUDEPATH += $$PWD/../../../

SOURCES +=  tst_trie_map_test.cpp

unix:!macx: LIBS += -L$$OUT_PWD/../../container/ -lcontainer

INCLUDEPATH += $$PWD/../../container
DEPENDPATH += $$PWD/../../container
