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

#ifndef SQL_PRIMITIVES_H
#define SQL_PRIMITIVES_H

#include <ctime>
#include <optional> // IWYU pragma: keep
#include <pqxx/pqxx>
#include <stdint.h>
#include <string>
#include <vector>

#include "postgres/query_runner/reflection/sql_primitive_interface.h"

namespace e8 {

/**
 * @brief The SqlBool class Boolean primitive type.
 *
 * Example usage:
 * SqlBool can_read = SqlBool("can_read");
 */
class SqlBool : public SqlPrimitiveInterface {
  public:
    explicit SqlBool(std::string const &field_name);
    explicit SqlBool(bool value, std::string const &field_name);
    ~SqlBool() override;
    SqlBool(SqlBool const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlBool &operator=(SqlBool const &rhs);

    /**
     * @brief value The internally stored C++ boolean value.
     */
    std::optional<bool> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<bool> *value_ptr();

  private:
    std::optional<bool> value_;
};

/**
 * @brief The SqlInt class 32-bit interger primitive type.
 *
 * Example usage:
 * SqlInt num_views = SqlInt("num_views");
 */
class SqlInt : public SqlPrimitiveInterface {
  public:
    explicit SqlInt(std::string const &field_name);
    explicit SqlInt(int32_t value, std::string const &field_name = "");
    ~SqlInt() override;
    SqlInt(SqlInt const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlInt &operator=(SqlInt const &rhs);

    /**
     * @brief value The internally stored C++ 32-bit integer.
     */
    std::optional<int32_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<int32_t> *value_ptr();

  private:
    std::optional<int32_t> value_;
};

/**
 * @brief The SqlLong class 64-bit integer primitive type.
 *
 * Example usage:
 * SqlLong num_views = SqlLong("num_views");
 */
class SqlLong : public SqlPrimitiveInterface {
  public:
    explicit SqlLong(std::string const &field_name);
    explicit SqlLong(int64_t value, std::string const &field_name = "");
    ~SqlLong() override;
    SqlLong(SqlLong const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlLong &operator=(SqlLong const &rhs);

    /**
     * @brief value The internally stored C++ 64-bit integer.
     */
    std::optional<int64_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<int64_t> *value_ptr();

  private:
    std::optional<int64_t> value_;
};

/**
 * @brief The SqlFloat class single-precision float primitive type.
 *
 * Example usage:
 * SqlFloat active_level = SqlFloat("active_level");
 */
class SqlFloat : public SqlPrimitiveInterface {
  public:
    explicit SqlFloat(std::string const &field_name);
    explicit SqlFloat(float value, std::string const &field_name = "");
    ~SqlFloat() override;
    SqlFloat(SqlFloat const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlFloat &operator=(SqlFloat const &rhs);

    /**
     * @brief value The internally stored C++ single-precision float.
     */
    std::optional<float> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<float> *value_ptr();

  private:
    std::optional<float> value_;
};

/**
 * @brief The SqlDouble class double-precision float primitive type.
 *
 * Example usage:
 * SqlDouble active_level = SqlDouble("active_level");
 */
class SqlDouble : public SqlPrimitiveInterface {
  public:
    explicit SqlDouble(std::string const &field_name);
    explicit SqlDouble(double value, std::string const &field_name = "");
    ~SqlDouble() override;
    SqlDouble(SqlDouble const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlDouble &operator=(SqlDouble const &rhs);

    /**
     * @brief value The internally stored C++ double-precision float.
     */
    std::optional<double> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<double> *value_ptr();

  private:
    std::optional<double> value_;
};

/**
 * @brief The SqlStr class String primitive type.
 *
 * Example usage:
 * SqlStr user_name = SqlStr("user_name");
 */
class SqlStr : public SqlPrimitiveInterface {
  public:
    explicit SqlStr(std::string const &field_name);
    SqlStr(std::string const &value, std::string const &field_name);
    ~SqlStr() override;
    SqlStr(SqlStr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlStr &operator=(SqlStr const &rhs);

    /**
     * @brief value The internally stored C++ string.
     */
    std::optional<std::string> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<std::string> *value_ptr();

  private:
    std::optional<std::string> value_;
};

/**
 * @brief The SqlTimestamp class Timestamp primitive type.
 *
 * Example usage:
 * SqlTimestamp created_at = SqlTimestamp("created_at");
 */
class SqlTimestamp : public SqlPrimitiveInterface {
  public:
    explicit SqlTimestamp(std::string const &field_name);
    explicit SqlTimestamp(std::time_t value, std::string const &field_name = "");
    ~SqlTimestamp() override;
    SqlTimestamp(SqlTimestamp const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlTimestamp &operator=(SqlTimestamp const &rhs);

    /**
     * @brief value The internally stored C++ time.
     */
    std::optional<std::time_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<std::time_t> *value_ptr();

  private:
    std::optional<std::time_t> value_;
};

/**
 * @brief The SqlBoolArr class boolean array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlBoolArr configurations = SqlBoolArr("configurations");
 */
class SqlBoolArr : public SqlPrimitiveInterface {
  public:
    explicit SqlBoolArr(std::string const &field_name);
    explicit SqlBoolArr(std::vector<bool> const &value, std::string const &field_name = "");
    ~SqlBoolArr() override;
    SqlBoolArr(SqlBoolArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlBoolArr &operator=(SqlBoolArr const &rhs);

    /**
     * @brief value The internally stored C++ boolean array.
     */
    std::vector<bool> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<bool> *value_ptr();

  private:
    std::vector<bool> value_;
};

/**
 * @brief The SqlIntArr class 32-bit integer array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlIntArr user_permissions = SqlIntArr("user_permissions");
 */
class SqlIntArr : public SqlPrimitiveInterface {
  public:
    explicit SqlIntArr(std::string const &field_name);
    explicit SqlIntArr(std::vector<int32_t> const &value, std::string const &field_name = "");
    ~SqlIntArr() override;
    SqlIntArr(SqlIntArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlIntArr &operator=(SqlIntArr const &rhs);

    /**
     * @brief value The internally stored C++ 32-bit integer array.
     */
    std::vector<int32_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<int32_t> *value_ptr();

  private:
    std::vector<int32_t> value_;
};

/**
 * @brief The SqlLongArr class 64-bit integer array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlLongArr user_ids = SqlLongArr("user_ids");
 */
class SqlLongArr : public SqlPrimitiveInterface {
  public:
    explicit SqlLongArr(std::string const &field_name);
    explicit SqlLongArr(std::vector<int64_t> const &value, std::string const &field_name = "");
    ~SqlLongArr() override;
    SqlLongArr(SqlLongArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlLongArr &operator=(SqlLongArr const &rhs);

    /**
     * @brief value The internally stored C++ 64-bit integer array.
     */
    std::vector<int64_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<int64_t> *value_ptr();

  private:
    std::vector<int64_t> value_;
};

/**
 * @brief The SqlFloatArr class single-precision float array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlFloatArr active_levels_last_week = SqlFloatArr("active_levels_last_week");
 */
class SqlFloatArr : public SqlPrimitiveInterface {
  public:
    explicit SqlFloatArr(std::string const &field_name);
    explicit SqlFloatArr(std::vector<float> const &value, std::string const &field_name = "");
    ~SqlFloatArr() override;
    SqlFloatArr(SqlFloatArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlFloatArr &operator=(SqlFloatArr const &rhs);

    /**
     * @brief value The internally stored C++ single-precision float array.
     */
    std::vector<float> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<float> *value_ptr();

  private:
    std::vector<float> value_;
};

/**
 * @brief The SqlDoubleArr class double-precision float array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlDoubleArr rectangle_vertices = SqlDoubleArr("rectangle_vertices");
 */
class SqlDoubleArr : public SqlPrimitiveInterface {
  public:
    explicit SqlDoubleArr(std::string const &field_name);
    explicit SqlDoubleArr(std::vector<double> const &value, std::string const &field_name = "");
    ~SqlDoubleArr() override;
    SqlDoubleArr(SqlDoubleArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlDoubleArr &operator=(SqlDoubleArr const &rhs);

    /**
     * @brief value The internally stored C++ double-precision float array.
     */
    std::vector<double> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<double> *value_ptr();

  private:
    std::vector<double> value_;
};

/**
 * @brief The SqlStrArr class string array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlStrArr recovery_emails = SqlStrArr("recovery_emails");
 */
class SqlStrArr : public SqlPrimitiveInterface {
  public:
    explicit SqlStrArr(std::string const &field_name);
    explicit SqlStrArr(std::vector<std::string> const &value, std::string const &field_name = "");
    ~SqlStrArr() override;
    SqlStrArr(SqlStrArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlStrArr &operator=(SqlStrArr const &rhs);

    /**
     * @brief value The internally stored C++ string array.
     */
    std::vector<std::string> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<std::string> *value_ptr();

  private:
    std::vector<std::string> value_;
};

/**
 * @brief The SqlTimestampArr class timestamp array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlTimestampArr last_4_login_timestamps = SqlTimestampArr("last_4_login_timestamps");
 */
class SqlTimestampArr : public SqlPrimitiveInterface {
  public:
    explicit SqlTimestampArr(std::string const &field_name);
    explicit SqlTimestampArr(std::vector<std::time_t> const &value,
                             std::string const &field_name = "");
    ~SqlTimestampArr() override;
    SqlTimestampArr(SqlTimestampArr const &) = default;

    void export_to_invocation(pqxx::prepare::invocation *invocation) const override;
    void import_from_field(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlTimestampArr &operator=(SqlTimestampArr const &rhs);

    /**
     * @brief value The internally stored C++ timestamp array.
     */
    std::vector<std::time_t> const &value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<std::time_t> *value_ptr();

  private:
    std::vector<std::time_t> value_;
};

} // namespace e8

#endif // SQL_PRIMITIVES_H
