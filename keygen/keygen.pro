#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T00:24:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = keygen
TEMPLATE = lib

DEFINES += KEYGEN_LIBRARY
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../

SOURCES += \
        key_generator_interface.cc \
    persistent_key_generator.cc \
    sign_message.cc

HEADERS += \
        key_generator_interface.h \
    persistent_key_generator.h \
    sign_message.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -L$$OUT_PWD/../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../postgres/query_runner
DEPENDPATH += $$PWD/../postgres/query_runner

LIBS += -lcrypto++

