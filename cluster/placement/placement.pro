CONFIG -= qt

TARGET = cluster_placement
TEMPLATE = lib

DEFINES += CLUSTER_PLACEMENT_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    bucket.cc \
    bucket_list.cc \
    bucket_straw.cc \
    bucket_tree.cc \
    bucket_uniform.cc \
    capability.cc \
    cluster_map.cc \
    hierarchy.cc \
    score.cc

HEADERS += \
    bucket.h \
    bucket_list.h \
    bucket_straw.h \
    bucket_tree.h \
    bucket_uniform.h \
    capability.h \
    cluster_map.h \
    common_types.h \
    hierarchy.h \
    score.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
