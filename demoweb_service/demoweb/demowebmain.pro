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

unix:!macx: LIBS += -L$$OUT_PWD/./ -ldemowebservice

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

unix:!macx: LIBS += -L$$OUT_PWD/../../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../../keygen
DEPENDPATH += $$PWD/../../keygen

unix:!macx: LIBS += -L$$OUT_PWD/../../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../../third_party/base64
DEPENDPATH += $$PWD/../../third_party/base64

LIBS += -lpthread
LIBS += -ldl
LIBS += -lgrpc++ -lgrpc++_reflection
