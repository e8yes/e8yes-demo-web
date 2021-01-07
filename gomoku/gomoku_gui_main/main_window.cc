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
#include <QWidget>
#include <cassert>
#include <memory>
#include <string>

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
    : QMainWindow(parent), board_(kWidth, kHeight), ui_(std::make_unique<Ui::MainWindow>()),
      gomoku_grid_(std::make_unique<QGridLayout>()),
      stones_(std::unique_ptr<QPushButton[]>(new QPushButton[kWidth * kHeight])),
      swap2_decision_choose_black_(std::make_unique<QPushButton>(parent)),
      swap2_decision_choose_white_(std::make_unique<QPushButton>(parent)),
      swap2_decision_place_2_stones_(std::make_unique<QPushButton>(parent)),
      stone_type_decision_choose_black_(std::make_unique<QPushButton>(parent)),
      stone_type_decision_choose_white_(std::make_unique<QPushButton>(parent)),
      game_phase_(std::make_unique<QLabel>(parent)),
      player_a_stone_type_(std::make_unique<QLabel>(parent)),
      player_b_stone_type_(std::make_unique<QLabel>(parent)),
      current_player_(std::make_unique<QLabel>(parent)),
      game_result_(std::make_unique<QLabel>(parent)) {
    ui_->setupUi(this);

    ui_->centralwidget->setLayout(gomoku_grid_.get());

    // Set labels.
    swap2_decision_choose_black_->setText("Choose black stone");
    swap2_decision_choose_white_->setText("Choose white stone");
    swap2_decision_place_2_stones_->setText("Place 2 stones");

    stone_type_decision_choose_black_->setText("Choose black stone");
    stone_type_decision_choose_white_->setText("Choose white stone");

    this->DisplayActions();
}

MainWindow::~MainWindow() {}

void MainWindow::DisplayActions() {
    ClearLayout(gomoku_grid_.get());

    GomokuAction const &sample_action = board_.LegalActions().begin()->second;
    if (sample_action.stone_pos.has_value()) {
        this->DisplayGomokuBoardActions();
    } else if (sample_action.swap2_decision.has_value()) {
        this->DisplaySwap2Actions();
    } else if (sample_action.stone_type_decision.has_value()) {
        this->DisplayStoneTypeActions();
    } else {
        assert(false);
    }
}

void MainWindow::DisplayGomokuBoardActions() {
    for (unsigned y = 0; y < kHeight; ++y) {
        for (unsigned x = 0; x < kWidth; ++x) {
            switch (board_.CurrentBoard()[x + y * kWidth]) {
            case ST_NONE: {
                stones_[x + y * kWidth].setText("-");
                break;
            }
            case ST_BLACK: {
                stones_[x + y * kWidth].setText("X");
                break;
            }
            case ST_WHITE: {
                stones_[x + y * kWidth].setText("O");
                break;
            }
            default: {
                assert(false);
                break;
            }
            }

            gomoku_grid_->addWidget(&stones_[x + y * kWidth], y, x, Qt::AlignCenter);
        }
    }

    this->DisplayCommonElements(/*beginning_column_index=*/kWidth);

    gomoku_grid_->update();
}

void MainWindow::DisplaySwap2Actions() {
    this->DisplayGomokuBoardActions();

    gomoku_grid_->addWidget(swap2_decision_choose_black_.get(), 0, kWidth, Qt::AlignCenter);
    gomoku_grid_->addWidget(swap2_decision_choose_white_.get(), 1, kWidth, Qt::AlignCenter);
    gomoku_grid_->addWidget(swap2_decision_place_2_stones_.get(), 2, kWidth, Qt::AlignCenter);

    this->DisplayCommonElements(/*beginning_column_index=*/kWidth + 1);

    gomoku_grid_->update();
}

void MainWindow::DisplayStoneTypeActions() {
    this->DisplayGomokuBoardActions();

    gomoku_grid_->addWidget(stone_type_decision_choose_black_.get(), 0, kWidth, Qt::AlignCenter);
    gomoku_grid_->addWidget(stone_type_decision_choose_white_.get(), 1, kWidth, Qt::AlignCenter);

    this->DisplayCommonElements(/*beginning_column_index=*/kWidth + 1);

    gomoku_grid_->update();
}

void MainWindow::DisplayCommonElements(int beginning_column_index) {
    switch (board_.CurrentGamePhase()) {
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

    switch (*board_.PlayerStoneType(e8::PlayerSide::PS_PLAYER_A)) {
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

    switch (*board_.PlayerStoneType(e8::PlayerSide::PS_PLAYER_B)) {
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

    switch (board_.CurrentPlayerSide()) {
    case PS_PLAYER_A: {
        current_player_->setText("Current Player: A");
        break;
    }
    case PS_PLAYER_B: {
        current_player_->setText("Current Player: B");
        break;
    }
    }

    switch (board_.CurrentGameResult()) {
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

} // namespace e8
