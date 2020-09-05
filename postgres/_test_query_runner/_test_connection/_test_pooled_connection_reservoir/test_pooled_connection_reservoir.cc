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

#include "common/unit_test_util/unit_test_util.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

bool TakeLessThanMaxAndPutBack() {
    e8::ConnectionFactory fact(e8::ConnectionFactory::PQ, /*host_name=*/"localhost",
                               /*db_name=*/"demoweb");
    e8::PooledConnectionReservoir reservoir(fact, /*max_conns=*/2);

    e8::ConnectionInterface *conn1 = reservoir.Take();
    TEST_CONDITION(conn1 != nullptr);
    e8::ConnectionInterface *conn2 = reservoir.Take();
    TEST_CONDITION(conn2 != nullptr);

    reservoir.Put(conn1);
    reservoir.Put(conn2);

    conn1 = reservoir.Take();
    TEST_CONDITION(conn1 != nullptr);
    conn2 = reservoir.Take();
    TEST_CONDITION(conn2 != nullptr);

    return true;
}

int main() {
    e8::BeginTestSuite("pooled_connection_reservoir");
    e8::RunTest("TakeLessThanMaxAndPutBack", TakeLessThanMaxAndPutBack);
    e8::EndTestSuite();
    return 0;
}
