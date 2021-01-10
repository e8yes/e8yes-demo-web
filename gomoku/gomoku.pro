TEMPLATE = subdirs
SUBDIRS = \        
        gomoku_game/gomoku_game.pro \
        gomoku_agent/gomoku_agent.pro \
        gomoku_gui_main/gomoku_gui_main.pro \
        _test_gomoku_game/_test_board/_test_board_state/_test_board_state.pro \
        _test_gomoku_agent/_test_heuristics/_test_light_rollout_evaluator/_test_light_rollout_evaluator.pro

CONFIG += ordered
