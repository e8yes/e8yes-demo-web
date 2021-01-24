QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

SOURCES += \
    agent_gui_player.cc \
    human_gui_player.cc \
    main.cc \
    main_window.cc

HEADERS += \
    agent_gui_player.h \
    human_gui_player.h \
    main_window.h

FORMS += \
    main_window.ui

TRANSLATIONS += \
    gomoku_gui_main_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../../common/container/ -lcontainer

INCLUDEPATH += $$PWD/../../common/container
DEPENDPATH += $$PWD/../../common/container

unix:!macx: LIBS += -L$$OUT_PWD/../../common/thread/ -lthread

INCLUDEPATH += $$PWD/../../common/thread
DEPENDPATH += $$PWD/../../common/thread

unix:!macx: LIBS += -L$$OUT_PWD/../../common/random/ -lrandom

INCLUDEPATH += $$PWD/../../common/random
DEPENDPATH += $$PWD/../../common/random

unix:!macx: LIBS += -L$$OUT_PWD/../game/ -lgomoku_game

INCLUDEPATH += $$PWD/../game
DEPENDPATH += $$PWD/../game

unix:!macx: LIBS += -L$$OUT_PWD/../agent/ -lgomoku_agent

INCLUDEPATH += $$PWD/../agent
DEPENDPATH += $$PWD/../agent

unix:!macx: LIBS += -L$$OUT_PWD/../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../common/time_util
DEPENDPATH += $$PWD/../../common/time_util
