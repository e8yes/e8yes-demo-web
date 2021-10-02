TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native -g
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

INCLUDEPATH += $$PWD/../../../

SOURCES += \
    test_capability.cc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../common/unit_test_util/release/ -lunit_test_util
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../common/unit_test_util/debug/ -lunit_test_util
else:unix: LIBS += -L$$OUT_PWD/../../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../../common/unit_test_util
DEPENDPATH += $$PWD/../../../common/unit_test_util

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../placement/release/ -lcluster_placement
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../placement/debug/ -lcluster_placement
else:unix: LIBS += -L$$OUT_PWD/../../placement/ -lcluster_placement

INCLUDEPATH += $$PWD/../../placement
DEPENDPATH += $$PWD/../../placement

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../proto_cc/release/ -lproto_cc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../proto_cc/debug/ -lproto_cc
else:unix: LIBS += -L$$OUT_PWD/../../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../../proto_cc
DEPENDPATH += $$PWD/../../../proto_cc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../third_party/uuid/release/ -luuid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../third_party/uuid/debug/ -luuid
else:unix: LIBS += -L$$OUT_PWD/../../../third_party/uuid/ -luuid

INCLUDEPATH += $$PWD/../../../third_party/uuid
DEPENDPATH += $$PWD/../../../third_party/uuid

LIBS += -lprotobuf
