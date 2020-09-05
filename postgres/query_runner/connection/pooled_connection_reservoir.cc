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

#include <cassert>
#include <ctime>
#include <mutex>
#include <queue>
#include <semaphore.h>

#include "postgres/query_runner/connection/connection_factory.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/connection/pooled_connection_reservoir.h"

namespace e8 {

PooledConnectionReservoir::PooledConnectionReservoir(ConnectionFactory const &fact,
                                                     unsigned const max_conns,
                                                     unsigned const expiry_duration_secs)
    : fact_(fact), max_conns_(max_conns), expiry_duration_secs_(expiry_duration_secs) {
    sem_init(&resource_count_, 0, max_conns);
}

PooledConnectionReservoir::~PooledConnectionReservoir() { CloseAll(); }

ConnectionInterface *PooledConnectionReservoir::Take() {
    sem_wait(&resource_count_);

    lock_.lock();
    std::time_t curr_timestamp;
    std::time(&curr_timestamp);

    while (!unused_pool_.empty() && unused_pool_.front().expiry_timestamp < curr_timestamp) {
        unused_pool_.pop();
    }

    if (unused_pool_.empty()) {
        unused_pool_.emplace(
            DatedConnection(fact_.Create(), curr_timestamp + expiry_duration_secs_));
    } else {
        // Refresh the expiry timestamp for the connection that is about to be activated.
        unused_pool_.front().expiry_timestamp = curr_timestamp + expiry_duration_secs_;
    }

    ConnectionInterface *conns = unused_pool_.front().conn.get();
    inused_pool_.insert(std::make_pair(conns, std::move(unused_pool_.front())));
    unused_pool_.pop();

    lock_.unlock();

    return conns;
}

void PooledConnectionReservoir::Put(ConnectionInterface *conn) {
    lock_.lock();

    auto it = inused_pool_.find(conn);
    if (it != inused_pool_.end()) {
        unused_pool_.emplace(std::move(it->second));
        inused_pool_.erase(it);
    }

    lock_.unlock();

    sem_post(&resource_count_);
}

void PooledConnectionReservoir::CloseAll() {
    lock_.lock();

    inused_pool_.clear();
    while (!unused_pool_.empty()) {
        unused_pool_.pop();
    }

    sem_init(&resource_count_, 0, max_conns_);

    lock_.unlock();
}

unsigned PooledConnectionReservoir::UnusedPoolSize() {
    lock_.lock();
    unsigned size = unused_pool_.size();
    lock_.unlock();

    return size;
}

unsigned PooledConnectionReservoir::InusedPoolSize() {
    lock_.lock();
    unsigned size = inused_pool_.size();
    lock_.unlock();

    return size;
}

} // namespace e8
