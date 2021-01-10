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

#include <QGridLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>

#include "gomoku/gomoku_gui_main/main_window.h"
#include "ui_main_window.h"

namespace e8 {
namespace {

void ClearLayout(QLayout *layout) {
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
        }
        delete item;
    }
}

} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(std::make_unique<Ui::MainWindow>()),
      event_looper_(std::make_unique<QTimer>(this)), gomoku_grid_(std::make_unique<QGridLayout>()),
      stones_(nullptr), swap2_decision_choose_black_(std::make_unique<QPushButton>(this)),
      swap2_decision_choose_white_(std::make_unique<QPushButton>(this)),
      swap2_decision_place_2_stones_(std::make_unique<QPushButton>(this)),
      stone_type_decision_choose_black_(std::make_unique<QPushButton>(this)),
      stone_type_decision_choose_white_(std::make_unique<QPushButton>(this)),
      game_phase_(std::make_unique<QLabel>(this)),
      player_a_stone_type_(std::make_unique<QLabel>(this)),
      player_b_stone_type_(std::make_unique<QLabel>(this)),
      current_player_(std::make_unique<QLabel>(this)),
      game_result_(std::make_unique<QLabel>(this)) {
    ui_->setupUi(this);
    ui_->centralwidget->setLayout(gomoku_grid_.get());

    connect(event_looper_.get(), &QTimer::timeout, this, &MainWindow::ProcessCommands);
    event_looper_->start(std::chrono::milliseconds(1));
}

MainWindow::~MainWindow() {}

void MainWindow::ScheduleInit(GomokuBoardState const &board_state, std::string const &player_side) {
    command_lock_.lock();
    commands_.push(Command(Command::CT_INIT, board_state, player_side));
    command_lock_.unlock();
}

void MainWindow::ScheduleUpdate(GomokuBoardState const &board_state) {
    command_lock_.lock();
    commands_.push(Command(Command::CT_UPDATE, board_state));
    command_lock_.unlock();
}

void MainWindow::ScheduleEnableActionInput(GomokuBoardState const &board_state) {
    command_lock_.lock();
    commands_.push(Command(Command::CT_ACTIVATE_ACTION_RETRIEVAL, board_state));
    command_lock_.unlock();
}

void MainWindow::ScheduleDisableActionActionInput(GomokuBoardState const &board_state) {
    command_lock_.lock();
    commands_.push(Command(Command::CT_DEACTIVATE_ACTION_RETRIEVAL, board_state));
    command_lock_.unlock();
}

void MainWindow::ScheduleGameEnding(GomokuBoardState const &board_state) {
    command_lock_.lock();
    commands_.push(Command(Command::CT_GAME_END, board_state));
    command_lock_.unlock();
}

GomokuActionId MainWindow::WaitForNextAction() {
    std::optional<GomokuActionId> action_id;

    do {
        command_lock_.lock();

        if (!commands_.empty() && commands_.front().call_type == Command::CT_ACTION_INPUT) {
            action_id = commands_.front().action_id;
            commands_.pop();
        }

        command_lock_.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } while (!action_id.has_value());

    return *action_id;
}

MainWindow::Command::Command(CallType const call_type, GomokuBoardState const &board_state,
                             std::string const &player_side)
    : call_type(call_type), board_state(board_state), player_side(player_side) {}

MainWindow::Command::Command(CallType const call_type, GomokuBoardState const &board_state)
    : call_type(call_type), board_state(board_state) {}

MainWindow::Command::Command(CallType const call_type, GomokuActionId const &action_id)
    : call_type(call_type), action_id(action_id) {}

void MainWindow::Init(GomokuBoardState const &board_state, std::string const &player_side) {
    this->setWindowTitle(QString::fromStdString(std::string("Gomoku Swap2 - ") + player_side));

    stones_ =
        std::unique_ptr<QPushButton[]>(new QPushButton[board_state.Width() * board_state.Height()]);

    // Set action ID as property.
    for (int y = 0; y < board_state.Height(); ++y) {
        for (int x = 0; x < board_state.Width(); ++x) {
            stones_[x + y * board_state.Width()].setProperty(
                "action_id", QVariant(board_state.MovePositionToActionId(MovePosition(x, y))));
        }
    }

    swap2_decision_choose_black_->setProperty(
        "action_id",
        QVariant(board_state.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_BLACK)));
    swap2_decision_choose_white_->setProperty(
        "action_id",
        QVariant(board_state.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_CHOOSE_WHITE)));
    swap2_decision_place_2_stones_->setProperty(
        "action_id",
        QVariant(board_state.Swap2DecisionToActionId(e8::Swap2Decision::SW2D_PLACE_2_STONES)));

    stone_type_decision_choose_black_->setProperty(
        "action_id",
        QVariant(board_state.StoneTypeDecisionToActionId(e8::StoneTypeDecision::STD_CHOOSE_BLACK)));
    stone_type_decision_choose_white_->setProperty(
        "action_id",
        QVariant(board_state.StoneTypeDecisionToActionId(e8::StoneTypeDecision::STD_CHOOSE_WHITE)));

    // Connect up the signals from the action elements to the action handler.
    for (int i = 0; i < board_state.Width() * board_state.Height(); ++i) {
        connect(&stones_[i], &QPushButton::clicked, this, &MainWindow::ActionHandler);
    }

    connect(swap2_decision_choose_black_.get(), &QPushButton::clicked, this,
            &MainWindow::ActionHandler);
    connect(swap2_decision_choose_white_.get(), &QPushButton::clicked, this,
            &MainWindow::ActionHandler);
    connect(swap2_decision_place_2_stones_.get(), &QPushButton::clicked, this,
            &MainWindow::ActionHandler);

    connect(stone_type_decision_choose_black_.get(), &QPushButton::clicked, this,
            &MainWindow::ActionHandler);
    connect(stone_type_decision_choose_white_.get(), &QPushButton::clicked, this,
            &MainWindow::ActionHandler);

    // Set labels.
    swap2_decision_choose_black_->setText("Choose black stone");
    swap2_decision_choose_white_->setText("Choose white stone");
    swap2_decision_place_2_stones_->setText("Place 2 stones");

    stone_type_decision_choose_black_->setText("Choose black stone");
    stone_type_decision_choose_white_->setText("Choose white stone");
}

void MainWindow::RenderGameState(GomokuBoardState const &board_state) {
    ClearLayout(gomoku_grid_.get());

    GomokuAction const &sample_action = board_state.LegalActions().begin()->second;
    if (sample_action.stone_pos.has_value()) {
        this->RenderBoard(board_state);
    } else if (sample_action.swap2_decision.has_value()) {
        this->RenderSwap2Buttons(board_state);
    } else if (sample_action.stone_type_decision.has_value()) {
        this->RenderStoneTypeButtons(board_state);
    } else {
        assert(false);
    }
}

void MainWindow::RenderBoard(GomokuBoardState const &board_state) {
    for (int y = 0; y < board_state.Height(); ++y) {
        for (int x = 0; x < board_state.Width(); ++x) {
            switch (board_state.CurrentBoard()[x + y * board_state.Width()]) {
            case ST_NONE: {
                stones_[x + y * board_state.Width()].setText("-");
                break;
            }
            case ST_BLACK: {
                stones_[x + y * board_state.Width()].setText("X");
                break;
            }
            case ST_WHITE: {
                stones_[x + y * board_state.Width()].setText("O");
                break;
            }
            default: {
                assert(false);
                break;
            }
            }

            gomoku_grid_->addWidget(&stones_[x + y * board_state.Width()], y, x, Qt::AlignCenter);
        }
    }

    this->RenderCommonElements(board_state, /*beginning_column_index=*/board_state.Width());

    gomoku_grid_->update();
}

void MainWindow::RenderSwap2Buttons(GomokuBoardState const &board_state) {
    this->RenderBoard(board_state);
    this->SetGomokuBoardLockState(board_state, /*lock=*/true);

    gomoku_grid_->addWidget(swap2_decision_choose_black_.get(), 0, board_state.Width(),
                            Qt::AlignCenter);
    gomoku_grid_->addWidget(swap2_decision_choose_white_.get(), 1, board_state.Width(),
                            Qt::AlignCenter);
    gomoku_grid_->addWidget(swap2_decision_place_2_stones_.get(), 2, board_state.Width(),
                            Qt::AlignCenter);

    this->RenderCommonElements(board_state, /*beginning_column_index=*/board_state.Width() + 1);

    gomoku_grid_->update();
}

void MainWindow::RenderStoneTypeButtons(GomokuBoardState const &board_state) {
    this->RenderBoard(board_state);
    this->SetGomokuBoardLockState(board_state, /*lock=*/true);

    gomoku_grid_->addWidget(stone_type_decision_choose_black_.get(), 0, board_state.Width(),
                            Qt::AlignCenter);
    gomoku_grid_->addWidget(stone_type_decision_choose_white_.get(), 1, board_state.Width(),
                            Qt::AlignCenter);

    this->RenderCommonElements(board_state, /*beginning_column_index=*/board_state.Width() + 1);

    gomoku_grid_->update();
}

void MainWindow::RenderCommonElements(GomokuBoardState const &board_state,
                                      int beginning_column_index) {
    switch (board_state.CurrentGamePhase()) {
    case GP_PLACE_3_STONES: {
        game_phase_->setText("Game phase: place 3 stones");
        break;
    }
    case GP_SWAP2_DECISION: {
        game_phase_->setText("Game phase: swap 2 decision");
        break;
    }
    case GP_SWAP2_PLACE_2_STONES: {
        game_phase_->setText("Game phase: place 2 more stones");
        break;
    }
    case GP_STONE_TYPE_DECISION: {
        game_phase_->setText("Game phase: stone type decision");
        break;
    }
    case GP_STANDARD_GOMOKU: {
        game_phase_->setText("Game phase: standard Gomoku");
        break;
    }
    }

    switch (board_state.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A)) {
    case ST_NONE: {
        player_a_stone_type_->setText("Player A stone type: undetermined");
        break;
    }
    case ST_BLACK: {
        player_a_stone_type_->setText("Player A stone type: X");
        break;
    }
    case ST_WHITE: {
        player_a_stone_type_->setText("Player A stone type: O");
        break;
    }
    default: {
        assert(false);
    }
    }

    switch (board_state.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B)) {
    case ST_NONE: {
        player_b_stone_type_->setText("Player B stone type: undetermined");
        break;
    }
    case ST_BLACK: {
        player_b_stone_type_->setText("Player B stone type: X");
        break;
    }
    case ST_WHITE: {
        player_b_stone_type_->setText("Player B stone type: O");
        break;
    }
    default: {
        assert(false);
    }
    }

    switch (board_state.CurrentPlayerSide()) {
    case PS_PLAYER_A: {
        current_player_->setText("Current Player: A");
        break;
    }
    case PS_PLAYER_B: {
        current_player_->setText("Current Player: B");
        break;
    }
    }

    switch (board_state.CurrentGameResult()) {
    case GR_TIE: {
        game_result_->setText("Game result: tie");
        break;
    }
    case GR_PLAYER_A_WIN: {
        game_result_->setText("Game result: player A won");
        break;
    }
    case GR_PLAYER_B_WIN: {
        game_result_->setText("Game result: player B won");
        break;
    }
    case GR_UNDETERMINED: {
        game_result_->setText("Game result: undetermined");
        break;
    }
    default: {
        assert(false);
        break;
    }
    }

    gomoku_grid_->addWidget(game_phase_.get(), 0, beginning_column_index, Qt::AlignLeft);
    gomoku_grid_->addWidget(player_a_stone_type_.get(), 1, beginning_column_index, Qt::AlignLeft);
    gomoku_grid_->addWidget(player_b_stone_type_.get(), 2, beginning_column_index, Qt::AlignLeft);
    gomoku_grid_->addWidget(current_player_.get(), 3, beginning_column_index, Qt::AlignLeft);
    gomoku_grid_->addWidget(game_result_.get(), 4, beginning_column_index, Qt::AlignLeft);
}

void MainWindow::SetGomokuBoardLockState(GomokuBoardState const &board_state, bool lock) {
    for (auto const &action : board_state.LegalActions()) {
        if (action.second.stone_pos.has_value()) {
            stones_[action.second.stone_pos->x + action.second.stone_pos->y * board_state.Width()]
                .setEnabled(!lock);
        }
    }

    gomoku_grid_->update();
}

void MainWindow::SetSwap2ButtonsLockState(GomokuBoardState const &board_state, bool lock) {
    if (!board_state.LegalActions().begin()->second.swap2_decision.has_value()) {
        return;
    }

    swap2_decision_choose_black_->setEnabled(!lock);
    swap2_decision_choose_white_->setEnabled(!lock);
    swap2_decision_place_2_stones_->setEnabled(!lock);

    gomoku_grid_->update();
}

void MainWindow::SetStoneTypeButtonsLockState(GomokuBoardState const &board_state, bool lock) {
    if (!board_state.LegalActions().begin()->second.stone_type_decision.has_value()) {
        return;
    }

    stone_type_decision_choose_black_->setEnabled(!lock);
    stone_type_decision_choose_white_->setEnabled(!lock);

    gomoku_grid_->update();
}

void MainWindow::ActionHandler() {
    QPushButton *button = static_cast<QPushButton *>(this->sender());

    command_lock_.lock();
    QVariant action_id = button->property("action_id");
    commands_.push(
        Command(Command::CT_ACTION_INPUT, *static_cast<GomokuActionId *>(action_id.data())));
    command_lock_.unlock();
}

void MainWindow::ProcessCommands() {
    command_lock_.lock();

    if (commands_.empty()) {
        command_lock_.unlock();
        return;
    }

    Command const &command = commands_.front();

    switch (command.call_type) {
    case Command::CT_INIT: {
        assert(command.board_state.has_value());
        assert(command.player_side.has_value());

        this->Init(*command.board_state, *command.player_side);
        this->RenderGameState(*command.board_state);

        commands_.pop();
        break;
    }
    case Command::CT_UPDATE: {
        assert(command.board_state.has_value());

        this->RenderGameState(*command.board_state);

        commands_.pop();
        break;
    }
    case Command::CT_ACTIVATE_ACTION_RETRIEVAL: {
        assert(command.board_state.has_value());

        this->SetGomokuBoardLockState(*command.board_state, /*lock=*/false);
        this->SetSwap2ButtonsLockState(*command.board_state, /*lock=*/false);
        this->SetStoneTypeButtonsLockState(*command.board_state, /*lock=*/false);

        commands_.pop();
        break;
    }
    case Command::CT_DEACTIVATE_ACTION_RETRIEVAL: {
        assert(command.board_state.has_value());

        this->SetGomokuBoardLockState(*command.board_state, /*lock=*/true);
        this->SetSwap2ButtonsLockState(*command.board_state, /*lock=*/true);
        this->SetStoneTypeButtonsLockState(*command.board_state, /*lock=*/true);

        commands_.pop();
        break;
    }
    case Command::CT_GAME_END: {
        // TODO: Notify game result.
        commands_.pop();
        break;
    }
    default: {
        assert(command.call_type == Command::CT_ACTION_INPUT);
        // Do nothing.
        break;
    }
    }

    command_lock_.unlock();
}

} // namespace e8
