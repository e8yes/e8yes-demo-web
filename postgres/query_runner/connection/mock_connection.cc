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

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "postgres/query_runner/connection/mock_connection.h"

namespace e8 {

MockConnection::MockQuerySetting::MockQuerySetting(ParameterizedQuery const &query,
                                                   QueryParams const &params,
                                                   MockResultSet const &result)
    : query(query), params(params), result(result) {}

bool MockConnection::MockQuerySetting::operator==(MockQuerySetting const &rhs) const {
    return query == rhs.query && params.parameters() == rhs.params.parameters();
}

MockConnection::MockUpdateSetting::MockUpdateSetting(ParameterizedQuery const &query,
                                                     QueryParams const &params,
                                                     uint64_t num_rows_affected)
    : query(query), params(params), num_rows_affected(num_rows_affected) {}

bool MockConnection::MockUpdateSetting::operator==(MockUpdateSetting const &rhs) const {
    return query == rhs.query && params.parameters() == rhs.params.parameters();
}

void MockConnection::set_query_result(ParameterizedQuery const &query, QueryParams const &params,
                                      MockResultSet const &result) {
    MockQuerySetting setting(query, params, result);

    auto it = std::find(mock_query_results_.begin(), mock_query_results_.end(), setting);
    if (it == mock_query_results_.end()) {
        mock_query_results_.push_back(setting);
    } else {
        *it = setting;
    }
}

void MockConnection::set_update_result(ParameterizedQuery const &query, QueryParams const &params,
                                       uint64_t const &num_rows_affected) {
    MockUpdateSetting setting(query, params, num_rows_affected);

    auto it = std::find(mock_update_results_.begin(), mock_update_results_.end(), setting);
    if (it == mock_update_results_.end()) {
        mock_update_results_.push_back(setting);
    } else {
        *it = setting;
    }
}

void MockConnection::set_closed(bool closed) { closed_ = closed; }

std::unique_ptr<ResultSetInterface> MockConnection::run_query(ParameterizedQuery const &query,
                                                              QueryParams const &params) {
    auto it = std::find_if(mock_query_results_.begin(), mock_query_results_.end(),
                           [&query, &params](MockQuerySetting const &setting) {
                               return query == setting.query &&
                                      params.parameters() == setting.params.parameters();
                           });
    assert(it != mock_query_results_.end());

    return std::make_unique<MockResultSet>(it->result);
}

uint64_t MockConnection::run_update(ParameterizedQuery const &query, QueryParams const &params) {
    auto it = std::find_if(mock_update_results_.begin(), mock_update_results_.end(),
                           [&query, &params](MockUpdateSetting const &setting) {
                               return query == setting.query &&
                                      params.parameters() == setting.params.parameters();
                           });
    assert(it != mock_update_results_.end());

    return it->num_rows_affected;
}

bool MockConnection::is_closed() const { return closed_; }

} // namespace e8
