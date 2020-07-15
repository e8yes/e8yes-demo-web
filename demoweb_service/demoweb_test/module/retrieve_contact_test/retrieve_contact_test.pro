QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17

INCLUDEPATH += $$PWD/../../../../

SOURCES +=  \
    tst_retrieve_contact_test.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../../../postgres/query_runner
DEPENDPATH += $$PWD/../../../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../../../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../../../../keygen
DEPENDPATH += $$PWD/../../../../keygen

unix:!macx: LIBS += -L$$OUT_PWD/../../../demoweb/ -ldemowebservice

INCLUDEPATH += $$PWD/../../../demoweb
DEPENDPATH += $$PWD/../../../demoweb

unix:!macx: LIBS += -L$$OUT_PWD/../../../../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../../../../third_party/base64
DEPENDPATH += $$PWD/../../../../third_party/base64
