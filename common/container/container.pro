#-------------------------------------------------
#
# Project created by QtCreator 2020-05-03T23:46:38
#
#-------------------------------------------------

QT       -= core gui

TARGET = container
TEMPLATE = lib

DEFINES += CONTAINER_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    lru_hash_map.cc \
    trie_map.cc

HEADERS += \
    lru_hash_map.h \
    trie_map.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
