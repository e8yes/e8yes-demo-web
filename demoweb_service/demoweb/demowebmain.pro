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
        main.cpp

unix:!macx: LIBS += -L$$OUT_PWD/./ -ldemowebservice

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

LIBS += -lpthread
LIBS += -ldl
LIBS += -lgrpc++ -lgrpc++_reflection
