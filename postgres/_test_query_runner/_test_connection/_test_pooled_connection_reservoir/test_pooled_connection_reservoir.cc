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

#include <thread>

#include "common/unit_test_util/unit_test_util.h"
#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"
#include "postgres/query_runner/sql_runner.h"

bool TakeLessThanMaxAndPutBack() {
    e8::ConnectionFactory fact(e8::ConnectionFactory::PQ, /*host_name=*/"localhost",
                               /*db_name=*/"demoweb");
    e8::PooledConnectionReservoir reservoir(fact, /*max_conns=*/2);

    e8::ConnectionInterface *conn1 = reservoir.Take();
    TEST_CONDITION(conn1 != nullptr);
    bool rc = e8::SendHeartBeat(conn1);
    TEST_CONDITION(rc == true);

    e8::ConnectionInterface *conn2 = reservoir.Take();
    TEST_CONDITION(conn2 != nullptr);
    rc = e8::SendHeartBeat(conn2);
    TEST_CONDITION(rc == true);

    reservoir.Put(conn1);
    reservoir.Put(conn2);

    conn1 = reservoir.Take();
    TEST_CONDITION(conn1 != nullptr);
    rc = e8::SendHeartBeat(conn1);
    TEST_CONDITION(rc == true);

    conn2 = reservoir.Take();
    TEST_CONDITION(conn2 != nullptr);
    rc = e8::SendHeartBeat(conn2);
    TEST_CONDITION(rc == true);

    return true;
}

bool ReuseConnectionTest() {
    e8::ConnectionFactory fact(e8::ConnectionFactory::PQ, /*host_name=*/"localhost",
                               /*db_name=*/"demoweb");
    e8::PooledConnectionReservoir reservoir(fact, /*max_conns=*/2);

    e8::ConnectionInterface *conn1 = reservoir.Take();
    TEST_CONDITION(conn1 != nullptr);
    bool rc = e8::SendHeartBeat(conn1);
    TEST_CONDITION(rc == true);

    reservoir.Put(conn1);

    e8::ConnectionInterface *conn2 = reservoir.Take();
    TEST_CONDITION(conn2 != nullptr);
    rc = e8::SendHeartBeat(conn2);
    TEST_CONDITION(rc == true);

    reservoir.Put(conn2);

    return true;
}

bool PoolSizesTest() {
    e8::ConnectionFactory fact(e8::ConnectionFactory::PQ, /*host_name=*/"localhost",
                               /*db_name=*/"demoweb");
    e8::PooledConnectionReservoir reservoir(fact, /*max_conns=*/2);

    TEST_CONDITION(reservoir.UnusedPoolSize() == 0);
    TEST_CONDITION(reservoir.InusedPoolSize() == 0);

    e8::ConnectionInterface *conn1 = reservoir.Take();
    TEST_CONDITION(reservoir.UnusedPoolSize() == 0);
    TEST_CONDITION(reservoir.InusedPoolSize() == 1);

    e8::ConnectionInterface *conn2 = reservoir.Take();
    TEST_CONDITION(reservoir.UnusedPoolSize() == 0);
    TEST_CONDITION(reservoir.InusedPoolSize() == 2);

    reservoir.Put(conn2);
    TEST_CONDITION(reservoir.UnusedPoolSize() == 1);
    TEST_CONDITION(reservoir.InusedPoolSize() == 1);

    conn2 = reservoir.Take();
    TEST_CONDITION(reservoir.UnusedPoolSize() == 0);
    TEST_CONDITION(reservoir.InusedPoolSize() == 2);

    reservoir.Put(conn1);
    TEST_CONDITION(reservoir.UnusedPoolSize() == 1);
    TEST_CONDITION(reservoir.InusedPoolSize() == 1);

    reservoir.Put(conn2);
    TEST_CONDITION(reservoir.UnusedPoolSize() == 2);
    TEST_CONDITION(reservoir.InusedPoolSize() == 0);

    return true;
}

e8::PooledConnectionReservoir *gReservoir;
e8::ConnectionInterface *gPrevConn;

void PutBackInTheFuture() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);

    gReservoir->Put(gPrevConn);
}

bool TakeConnectionFromTheFuture() {
    e8::ConnectionFactory fact(e8::ConnectionFactory::PQ, /*host_name=*/"localhost",
                               /*db_name=*/"demoweb");
    e8::PooledConnectionReservoir reservoir(fact, /*max_conns=*/1);
    gReservoir = &reservoir;

    std::thread thr(PutBackInTheFuture);

    gPrevConn = reservoir.Take();
    e8::ConnectionInterface *future_conn = reservoir.Take();
    reservoir.Put(future_conn);

    thr.join();

    return true;
}

int main() {
    e8::BeginTestSuite("pooled_connection_reservoir");
    e8::RunTest("TakeLessThanMaxAndPutBack", TakeLessThanMaxAndPutBack);
    e8::RunTest("ReuseConnectionTest", ReuseConnectionTest);
    e8::RunTest("PoolSizesTest", PoolSizesTest);
    e8::RunTest("TakeConnectionFromTheFuture", TakeConnectionFromTheFuture);
    e8::EndTestSuite();
    return 0;
}
