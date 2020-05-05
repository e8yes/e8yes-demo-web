QT       -= core gui
TARGET = demowebservice
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

HEADERS += \
    module_identity/create_user.h \
    module_identity/user_entity.h \
    constant/table_names.h \
    module_rbac/system_user_group.h
SOURCES += \
    module_identity/create_user.cc \
    module_identity/user_entity.cc

unix:!macx: LIBS += -L$$OUT_PWD/../../postgres/query_runner/ -lquery_runner

INCLUDEPATH += $$PWD/../../postgres/query_runner
DEPENDPATH += $$PWD/../../postgres/query_runner

LIBS += -lpqxx
LIBS += -lpthread
LIBS += -ldl
LIBS += -lgrpc++ -lgrpc++_reflection
