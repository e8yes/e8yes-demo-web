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

#ifndef POOLEDCONNECTIONRESERVOIR_H
#define POOLEDCONNECTIONRESERVOIR_H

#include <ctime>
#include <mutex>
#include <queue>
#include <semaphore.h>
#include <unordered_map>

#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

static unsigned const kPooledConnectionSizeLimit = 10;
static unsigned const kPooledConnectionExpiryDurationSecs = 60 * 10;

/**
 * @brief The PooledConnectionReservoir class Maintains a connection pool with a specified size
 * limit. This allows connections to be reused as many times as possible.
 */
class PooledConnectionReservoir : public ConnectionReservoirInterface {
  public:
    /**
     * @brief PooledConnectionReservoir
     * @param max_conns The size limit of the connection pool.
     * @param expiry_duration_secs The maximum lifespan of each connection in the pool.
     */
    PooledConnectionReservoir(
        ConnectionFactory const &fact, unsigned const max_conns = kPooledConnectionSizeLimit,
        unsigned const expiry_duration_secs = kPooledConnectionExpiryDurationSecs);
    ~PooledConnectionReservoir() override;

    ConnectionInterface *Take() override;

    void Put(ConnectionInterface *conn) override;

    void CloseAll() override;

    /**
     * @brief UnusedPoolSize Current number of connections remains unused in the pool.
     */
    unsigned UnusedPoolSize();

    /**
     * @brief InusedPoolSize Current number of connections that are inused.
     */
    unsigned InusedPoolSize();

  private:
    struct DatedConnection {
        DatedConnection(std::unique_ptr<ConnectionInterface> conn, std::time_t expiry_timestamp)
            : conn(std::move(conn)), expiry_timestamp(expiry_timestamp) {}
        DatedConnection(DatedConnection &&) = default;

        std::unique_ptr<ConnectionInterface> conn;
        std::time_t expiry_timestamp;
    };

    ConnectionFactory fact_;

    std::queue<DatedConnection> unused_pool_;
    std::unordered_map<ConnectionInterface *, DatedConnection> inused_pool_;

    sem_t resource_count_;
    std::mutex lock_;

    unsigned const max_conns_;
    unsigned const expiry_duration_secs_;
};

} // namespace e8

#endif // POOLEDCONNECTIONRESERVOIR_H
