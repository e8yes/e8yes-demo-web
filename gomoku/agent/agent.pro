QT       -= core gui
TARGET = gomoku_agent
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += GOMOKU_AGENT_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

SOURCES += \
    heuristics/contour.cc \
    heuristics/evaluator.cc \
    heuristics/light_rollout_evaluator.cc \
    heuristics/shl_feature.cc \
    heuristics/shl_rollout_evaluator.cc \
    heuristics/tf_zero_prior_evaluator.cc \
    heuristics/tflite_zero_prior_evaluator.cc \
    mcts_agent_player.cc \
    search/mct_node.cc \
    search/mct_search.cc

HEADERS += \
    heuristics/contour.h \
    heuristics/evaluator.h \
    heuristics/light_rollout_evaluator.h \
    heuristics/shl_feature.h \
    heuristics/shl_rollout_evaluator.h \
    heuristics/tf_zero_prior_evaluator.h \
    heuristics/tflite_zero_prior_evaluator.h \
    mcts_agent_player.h \
    search/mct_node.h \
    search/mct_search.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
