/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QTimer>
#include <QWidget>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

#include "gomoku/game/board_state.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace e8 {

/**
 * @brief The MainWindow class A Qt GUI window to present the Gomoku game for the human player. It's
 * assumed that the GUI is run in a different thread from the main game logic. Its functions only
 * pass an event to the GUI thread from the main game logic rather than executing the function
 * immediately.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief ScheduleInit Schedule a game initialization event.
     */
    void ScheduleInit(GomokuBoardState const &board_state, std::string const &player_side);

    /**
     * @brief ScheduleUpdate Schedule a game state update event.
     */
    void ScheduleUpdate(GomokuBoardState const &board_state);

    /**
     * @brief ScheduleGameEnding Schedule a game ending event.
     */
    void ScheduleGameEnding(GomokuBoardState const &board_state);

    /**
     * @brief ScheduleActionInput Schedule to enable user input.
     */
    void ScheduleEnableActionInput(GomokuBoardState const &board_state);

    /**
     * @brief ScheduleDisableActionActionInput Schedule to disable user input.
     */
    void ScheduleDisableActionActionInput(GomokuBoardState const &board_state);

    /**
     * @brief WaitForNextAction Wait for the user input until one is made.
     */
    GomokuActionId WaitForNextAction();

  private:
    struct Command {
        enum CallType {
            CT_INIT,
            CT_UPDATE,
            CT_ACTIVATE_ACTION_RETRIEVAL,
            CT_DEACTIVATE_ACTION_RETRIEVAL,
            CT_ACTION_INPUT,
            CT_GAME_END
        };

        CallType call_type;
        std::optional<GomokuBoardState> board_state;
        std::optional<GomokuActionId> action_id;
        std::optional<std::string> player_side;

        Command(CallType const call_type, GomokuBoardState const &board_state,
                std::string const &player_side);
        Command(CallType const call_type, GomokuBoardState const &board_state);
        Command(CallType const call_type, GomokuActionId const &action_id);
    };

    void Init(GomokuBoardState const &board_state, std::string const &player_side);
    void RenderGameState(GomokuBoardState const &board_state);

    void RenderBoard(GomokuBoardState const &board_state);
    void RenderSwap2Buttons(GomokuBoardState const &board_state);
    void RenderStoneTypeButtons(GomokuBoardState const &board_state);
    void RenderCommonElements(GomokuBoardState const &board_state, int beginning_column_index);

    void SetGomokuBoardLockState(GomokuBoardState const &board_state, bool lock);
    void SetSwap2ButtonsLockState(GomokuBoardState const &board_state, bool lock);
    void SetStoneTypeButtonsLockState(GomokuBoardState const &board_state, bool lock);

    // UI elements.
    std::unique_ptr<Ui::MainWindow> ui_;

    std::unique_ptr<QTimer> event_looper_;

    std::unique_ptr<QGridLayout> gomoku_grid_;
    std::unique_ptr<QPushButton[]> stones_;

    std::unique_ptr<QPushButton> swap2_decision_choose_black_;
    std::unique_ptr<QPushButton> swap2_decision_choose_white_;
    std::unique_ptr<QPushButton> swap2_decision_place_2_stones_;

    std::unique_ptr<QPushButton> stone_type_decision_choose_black_;
    std::unique_ptr<QPushButton> stone_type_decision_choose_white_;

    std::unique_ptr<QLabel> game_phase_;
    std::unique_ptr<QLabel> player_a_stone_type_;
    std::unique_ptr<QLabel> player_b_stone_type_;
    std::unique_ptr<QLabel> current_player_;
    std::unique_ptr<QLabel> game_result_;

    std::mutex command_lock_;
    std::queue<Command> commands_;

  private slots:
    void ActionHandler();

    void ProcessCommands();
};

} // namespace e8

#endif // MAINWINDOW_H
