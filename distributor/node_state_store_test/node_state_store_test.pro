QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += $$PWD/../../

TEMPLATE = app

SOURCES +=  \
    tst_node_state_store_test.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../store/ -lnode_state_store

INCLUDEPATH += $$PWD/../store
DEPENDPATH += $$PWD/../store

LIBS += -lprotobuf
LIBS += -lsqlite3
