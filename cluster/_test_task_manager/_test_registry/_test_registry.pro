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
    test_registry.cc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../common/unit_test_util/release/ -lunit_test_util
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../common/unit_test_util/debug/ -lunit_test_util
else:unix: LIBS += -L$$OUT_PWD/../../../common/unit_test_util/ -lunit_test_util

INCLUDEPATH += $$PWD/../../../common/unit_test_util
DEPENDPATH += $$PWD/../../../common/unit_test_util

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../task_manager/release/ -lcluster_task_manager
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../task_manager/debug/ -lcluster_task_manager
else:unix: LIBS += -L$$OUT_PWD/../../task_manager/ -lcluster_task_manager

INCLUDEPATH += $$PWD/../../task_manager
DEPENDPATH += $$PWD/../../task_manager

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../proto_cc/release/ -lproto_cc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../proto_cc/debug/ -lproto_cc
else:unix: LIBS += -L$$OUT_PWD/../../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../../proto_cc
DEPENDPATH += $$PWD/../../../proto_cc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../common/thread/release/ -lthread
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../common/thread/debug/ -lthread
else:unix: LIBS += -L$$OUT_PWD/../../../common/thread/ -lthread

INCLUDEPATH += $$PWD/../../../common/thread
DEPENDPATH += $$PWD/../../../common/thread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../common/time_util/release/ -ltime_util
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../common/time_util/debug/ -ltime_util
else:unix: LIBS += -L$$OUT_PWD/../../../common/time_util/ -ltime_util

INCLUDEPATH += $$PWD/../../../common/time_util
DEPENDPATH += $$PWD/../../../common/time_util

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../third_party/uuid/release/ -luuid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../third_party/uuid/debug/ -luuid
else:unix: LIBS += -L$$OUT_PWD/../../../third_party/uuid/ -luuid

INCLUDEPATH += $$PWD/../../../third_party/uuid
DEPENDPATH += $$PWD/../../../third_party/uuid

LIBS += -lgrpc++
LIBS += -lgrpc++_reflection
LIBS += -lprotobuf
LIBS += -pthread
