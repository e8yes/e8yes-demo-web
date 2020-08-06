QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

TEMPLATE = app

INCLUDEPATH += $$PWD/../../../../

SOURCES +=  \
    tst_message_queue_store_test.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../../../proto_cc
DEPENDPATH += $$PWD/../../../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../message_queue/ -lmessage_queue_service

INCLUDEPATH += $$PWD/../../../message_queue
DEPENDPATH += $$PWD/../../../message_queue

LIBS += -pthread
LIBS += -lprotobuf
