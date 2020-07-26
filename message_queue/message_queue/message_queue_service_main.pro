TEMPLATE = app

qt -= core

CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

INCLUDEPATH += $$PWD/../../

SOURCES += \
    main.cc

unix:!macx: LIBS += -L$$OUT_PWD/./ -lmessage_queue_service

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

LIBS += -pthread
LIBS += -ldl
LIBS += -lprotobuf
LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
