QT       -= core gui
TARGET = demowebservice
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Ofast -flto -march=native -mtune=native
QMAKE_LDFLAGS_RELEASE += -Ofast -flto -march=native -mtune=native

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    sql/sql_query_builder.h \
    sql/connection/connection_interface.h \
    sql/reflection/sql_primitive_interface.h \
    sql/resultset/result_set_interface.h \
    sql/reflection/sql_entity_interface.h \
    sql/reflection/sql_primitives.h \
    sql/reflection/sql_primitives.h \
    sql/resultset/pq_result_set.h \
    sql/connection/pq_connection.h \
    util/trie_map.h \
    util/lru_hash_map.h

SOURCES += \
    sql/sql_query_builder.cc \
    sql/connection/connection_interface.cc \
    sql/reflection/sql_primitive_interface.cc \
    sql/reflection/sql_entity_interface.cc \
    sql/reflection/sql_primitives.cc \
    sql/resultset/result_set_interface.cc \
    sql/resultset/pq_result_set.cc \
    sql/connection/pq_connection.cc \
    util/trie_map.cc \
    util/lru_hash_map.cc

LIBS += -lpqxx
