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
#include <QPushButton>
#include <memory>

#include "gomoku/gomoku_game/board_state.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace e8 {

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    void DisplayActions();

    void DisplayGomokuBoardActions(bool enabled);

    void DisplaySwap2Actions();

    void DisplayStoneTypeActions();

    void DisplayCommonElements(int beginning_column_index);

    unsigned const kWidth = 11;
    unsigned const kHeight = 11;

    // Game elements.
    GomokuBoardState board_;

    // UI elements.
    std::unique_ptr<Ui::MainWindow> ui_;

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
};

} // namespace e8

#endif // MAINWINDOW_H
