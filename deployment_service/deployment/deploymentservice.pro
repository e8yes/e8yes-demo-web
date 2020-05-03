#-------------------------------------------------
#
# Project created by QtCreator 2020-05-01T19:12:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = deploymentservice
TEMPLATE = lib

CONFIG -= qt
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += DEPLOYMENTSERVICE_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    deployment_service.cc \
    deployment_service.pb.cc \
    deployment_service.grpc.pb.cc

HEADERS += \
    deployment_service.h \
    deployment_service.pb.h \
    deployment_service.grpbc.pb.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lpthread
LIBS += -ldl
LIBS += -lgrpc++ -lgrpc++_reflection
