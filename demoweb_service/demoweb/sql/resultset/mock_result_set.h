#ifndef MOCK_RESULT_SET_H
#define MOCK_RESULT_SET_H

#include <memory>
#include <vector>

#include "sql/reflection/sql_primitive_interface.h"
#include "sql/resultset/result_set_interface.h"

namespace e8 {

/**
 * @brief The MockResultSet class This implementations tailors to testing purposes
 */
class MockResultSet : public ResultSetInterface {
  public:
    /**
     * @brief MockResultSet
     * @param num_cells The length of each record in the result set.
     */
    MockResultSet(unsigned num_cells);
    MockResultSet(MockResultSet const &) = default;
    ~MockResultSet() override = default;

    // The length of each record should be the size of num_cells specified in the constructor.
    using Record = std::vector<std::shared_ptr<SqlPrimitiveInterface>>;

    /**
     * @brief add_record Append a record to the mock result set. This is used to prepare test data
     * before calling the ResultSetInterface functions.
     * @param record A record to be appended to the result set.
     */
    void add_record(Record const &record);

    void next() override;
    bool has_next() const override;
    void set_field(unsigned i, SqlPrimitiveInterface *field) override;

  private:
    std::vector<Record> records_;
    unsigned num_cells_;
    unsigned cur_record_ = 0;
};

} // namespace e8

#endif // MOCK_RESULT_SET_H
