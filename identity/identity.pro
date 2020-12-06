CONFIG -= qt

TEMPLATE = lib
DEFINES += IDENTITY_LIBRARY
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
    extract_identity_from_metadata.cc \
    trustable_identity.cc

HEADERS += \
    auth_key.h \
    extract_identity_from_metadata.h \
    trustable_identity.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../proto_cc
DEPENDPATH += $$PWD/../proto_cc

unix:!macx: LIBS += -L$$OUT_PWD/../third_party/base64/ -lbase64

INCLUDEPATH += $$PWD/../third_party/base64
DEPENDPATH += $$PWD/../third_party/base64

unix:!macx: LIBS += -L$$OUT_PWD/../keygen/ -lkeygen

INCLUDEPATH += $$PWD/../keygen
DEPENDPATH += $$PWD/../keygen

LIBS += -lprotobuf
LIBS += -lgrpc++
