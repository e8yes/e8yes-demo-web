/**
 * e8yes demo web server.
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

#include <QtTest>

#include "demoweb_service/demoweb/module_identity/create_user.h"

class create_user_test : public QObject {
    Q_OBJECT

  public:
    create_user_test();
    ~create_user_test();

  private slots:
    void create_baseline_user_test();
};

create_user_test::create_user_test() {}

create_user_test::~create_user_test() {}

void create_user_test::create_baseline_user_test() {}

QTEST_APPLESS_MAIN(create_user_test)

#include "tst_create_user_test.moc"
