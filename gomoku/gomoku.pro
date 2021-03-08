TEMPLATE = subdirs
SUBDIRS = \
        game/game.pro \
        logging/logging.pro \
        agent/agent.pro \
        agent_classroom/agent_classroom.pro \
        agent_classroom/representative_data_main.pro \
        agent_classroom/policy_iterator_main.pro \
        service/gomoku_service.pro \
        gui_main/gui_main.pro \
        _test_game/_test_board_state/_test_board_state.pro \
        _test_game/_test_game_instance_container/_test_game_instance_container.pro \
        _test_agent/_test_heuristics/_test_contour/_test_contour.pro \
        _test_agent/_test_heuristics/_test_shl_feature/_test_shl_feature.pro \
        _test_agent/_test_heuristics/_test_light_rollout_evaluator/_test_light_rollout_evaluator.pro \
        _test_agent/_test_heuristics/_test_tflite_zero_prior_evaluator/_test_tflite_zero_prior_evaluator.pro \
        _test_agent/_test_heuristics/_test_tf_zero_prior_evaluator/_test_tf_zero_prior_evaluator.pro \
        _test_agent/_test_heuristics/_test_shl_model_evaluator/_test_shl_model_evaluator.pro \
        _test_agent/_test_search/_test_mct_search/_test_mct_search.pro

CONFIG += ordered
