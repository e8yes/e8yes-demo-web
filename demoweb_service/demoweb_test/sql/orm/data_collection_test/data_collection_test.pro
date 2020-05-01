QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

TEMPLATE = app

SOURCES +=  tst_data_collection_test.cpp

unix:!macx: LIBS += -L$$OUT_PWD/../../../../demoweb/ -ldemowebservice

INCLUDEPATH += $$PWD/../../../../demoweb
DEPENDPATH += $$PWD/../../../../demoweb
