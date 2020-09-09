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

#ifndef MOCK_CONNECTION_H
#define MOCK_CONNECTION_H

#include <cstdint>
#include <memory>
#include <vector>

#include "postgres/query_runner/connection/connection_interface.h"
#include "postgres/query_runner/resultset/mock_result_set.h"
#include "postgres/query_runner/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The MockConnection class This implementations tailors to testing purposes.
 */
class MockConnection : public ConnectionInterface {
  public:
    MockConnection() = default;
    MockConnection(MockConnection const &) = delete;
    ~MockConnection() override = default;

    /**
     * @brief set_query_result Set the query result corresponding to the query and its parameteres.
     * Repeated call of this function will override the previous setting. The program will fail, if
     * the query result is not set before calling the below run_query() function.
     */
    void SetQueryResult(ParameterizedQuery const &query, QueryParams const &params,
                        MockResultSet const &result);

    /**
     * @brief set_update_result Set the number of rows affected by the corresponding update query
     * and its parameters. Repeated call of this function will override the previous setting.  The
     * program will fail, if the query result is not set before calling the below run_update()
     * function.
     */
    void SetUpdateResult(ParameterizedQuery const &query, QueryParams const &params,
                         uint64_t const &num_rows_affected);

    /**
     * @brief set_closed The the close state of this connection. Marking it closed will affect the
     * result of is_closed().
     * @param closed The new state of the this connection.
     */
    void SetClosed(bool closed);

    std::unique_ptr<ResultSetInterface> RunQuery(ParameterizedQuery const &query,
                                                 QueryParams const &params,
                                                 bool cache_on = true) override;

    uint64_t RunUpdate(ParameterizedQuery const &query, QueryParams const &params,
                       bool cache_on = true) override;

    bool IsClosed() const override;

  private:
    struct MockQuerySetting {
        MockQuerySetting(ParameterizedQuery const &query, QueryParams const &params,
                         MockResultSet const &result);
        MockQuerySetting(MockQuerySetting const &) = default;
        ~MockQuerySetting() = default;

        bool operator==(MockQuerySetting const &rhs) const;

        ParameterizedQuery query;
        QueryParams params;
        MockResultSet result;
    };

    struct MockUpdateSetting {
        MockUpdateSetting(ParameterizedQuery const &query, QueryParams const &params,
                          uint64_t num_rows_affected);
        MockUpdateSetting(MockUpdateSetting const &) = default;
        ~MockUpdateSetting() = default;

        bool operator==(MockUpdateSetting const &rhs) const;

        ParameterizedQuery query;
        QueryParams params;
        uint64_t num_rows_affected;
    };

    std::vector<MockQuerySetting> mock_query_results_;
    std::vector<MockUpdateSetting> mock_update_results_;
    bool closed_ = false;
    int8_t padding_[7];
};

} // namespace e8

#endif // MOCK_CONNECTION_H
