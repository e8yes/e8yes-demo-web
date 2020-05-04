#-------------------------------------------------
#
# Project created by QtCreator 2020-05-03T23:32:41
#
#-------------------------------------------------

QT       -= core gui

TARGET = query_runner
TEMPLATE = lib

DEFINES += QUERY_RUNNER_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    connection/basic_connection_reservoir.cc \
    connection/connection_factory.cc \
    connection/connection_interface.cc \
    connection/connection_reservoir_interface.cc \
    connection/mock_connection.cc \
    connection/pq_connection.cc \
    orm/data_collection.cc \
    orm/query_completion.cc \
    reflection/sql_entity_interface.cc \
    reflection/sql_primitive_interface.cc \
    reflection/sql_primitives.cc \
    resultset/mock_result_set.cc \
    resultset/pq_result_set.cc \
    resultset/result_set_interface.cc \
    sql_query_builder.cc \
    sql_runner.cc

HEADERS += \
    connection/basic_connection_reservoir.h \
    connection/connection_factory.h \
    connection/connection_interface.h \
    connection/connection_reservoir_interface.h \
    connection/mock_connection.h \
    connection/pq_connection.h \
    orm/data_collection.h \
    orm/query_completion.h \
    reflection/sql_entity_interface.h \
    reflection/sql_primitive_interface.h \
    reflection/sql_primitives.h \
    resultset/mock_result_set.h \
    resultset/pq_result_set.h \
    resultset/result_set_interface.h \
    sql_query_builder.h \
    sql_runner.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -lpqxx
