TEMPLATE = app
CONFIG += console

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

SOURCES += \
    representative_data_main.cc

unix:!macx: LIBS += -L$$OUT_PWD/./ -lgomoku_agent_classroom

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$OUT_PWD/../../common/flags/ -lflags

INCLUDEPATH += $$PWD/../../common/flags
DEPENDPATH += $$PWD/../../common/flags

unix:!macx: LIBS += -L$$OUT_PWD/../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../common/time_util
DEPENDPATH += $$PWD/../../common/time_util

unix:!macx: LIBS += -L$$OUT_PWD/../../common/container/ -lcontainer

INCLUDEPATH += $$PWD/../../common/container
DEPENDPATH += $$PWD/../../common/container

unix:!macx: LIBS += -L$$OUT_PWD/../../common/thread/ -lthread

INCLUDEPATH += $$PWD/../../common/thread
DEPENDPATH += $$PWD/../../common/thread

unix:!macx: LIBS += -L$$OUT_PWD/../../common/random/ -lrandom

INCLUDEPATH += $$PWD/../../common/random
DEPENDPATH += $$PWD/../../common/random

unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../logging/ -lgomoku_logging

INCLUDEPATH += $$PWD/../logging
DEPENDPATH += $$PWD/../logging

unix:!macx: LIBS += -L$$OUT_PWD/../game/ -lgomoku_game

INCLUDEPATH += $$PWD/../game
DEPENDPATH += $$PWD/../game

unix:!macx: LIBS += -L$$OUT_PWD/../agent/ -lgomoku_agent

INCLUDEPATH += $$PWD/../agent
DEPENDPATH += $$PWD/../agent

LIBS += -ltensorflowlite_c
