QT       -= core gui
TARGET = gomoku_service
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += GOMOKU_SERVICE_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

SOURCES +=
HEADERS +=

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../game/ -lgomoku_game

INCLUDEPATH += $$PWD/../game
DEPENDPATH += $$PWD/../game

unix:!macx: LIBS += -L$$OUT_PWD/../agent_classroom/ -lgomoku_agent_classroom

INCLUDEPATH += $$PWD/../agent_classroom
DEPENDPATH += $$PWD/../agent_classroom

unix:!macx: LIBS += -L$$OUT_PWD/../../distributor/distributor/ -ldistributor

INCLUDEPATH += $$PWD/../../distributor/distributor
DEPENDPATH += $$PWD/../../distributor/distributor
