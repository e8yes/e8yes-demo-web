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
    explicit SqlBool(std::string const &FieldName);
    explicit SqlBool(bool Value, std::string const &FieldName);
    ~SqlBool() override;
    SqlBool(SqlBool const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlBool &operator=(SqlBool const &rhs);

    /**
     * @brief value The internally stored C++ boolean value.
     */
    std::optional<bool> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<bool> *ValuePtr();

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
    explicit SqlInt(std::string const &FieldName);
    explicit SqlInt(int32_t Value, std::string const &FieldName = "");
    ~SqlInt() override;
    SqlInt(SqlInt const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlInt &operator=(SqlInt const &rhs);

    /**
     * @brief value The internally stored C++ 32-bit integer.
     */
    std::optional<int32_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<int32_t> *ValuePtr();

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
    explicit SqlLong(std::string const &FieldName);
    explicit SqlLong(int64_t Value, std::string const &FieldName = "");
    ~SqlLong() override;
    SqlLong(SqlLong const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlLong &operator=(SqlLong const &rhs);

    /**
     * @brief value The internally stored C++ 64-bit integer.
     */
    std::optional<int64_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<int64_t> *ValuePtr();

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
    explicit SqlFloat(std::string const &FieldName);
    explicit SqlFloat(float Value, std::string const &FieldName = "");
    ~SqlFloat() override;
    SqlFloat(SqlFloat const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlFloat &operator=(SqlFloat const &rhs);

    /**
     * @brief value The internally stored C++ single-precision float.
     */
    std::optional<float> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<float> *ValuePtr();

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
    explicit SqlDouble(std::string const &FieldName);
    explicit SqlDouble(double Value, std::string const &FieldName = "");
    ~SqlDouble() override;
    SqlDouble(SqlDouble const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlDouble &operator=(SqlDouble const &rhs);

    /**
     * @brief value The internally stored C++ double-precision float.
     */
    std::optional<double> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<double> *ValuePtr();

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
    explicit SqlStr(std::string const &FieldName);
    SqlStr(std::string const &Value, std::string const &FieldName);
    ~SqlStr() override;
    SqlStr(SqlStr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlStr &operator=(SqlStr const &rhs);

    /**
     * @brief value The internally stored C++ string.
     */
    std::optional<std::string> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<std::string> *ValuePtr();

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
    explicit SqlTimestamp(std::string const &FieldName);
    explicit SqlTimestamp(std::time_t Value, std::string const &FieldName = "");
    ~SqlTimestamp() override;
    SqlTimestamp(SqlTimestamp const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlTimestamp &operator=(SqlTimestamp const &rhs);

    /**
     * @brief value The internally stored C++ time.
     */
    std::optional<std::time_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::optional<std::time_t> *ValuePtr();

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
    explicit SqlBoolArr(std::string const &FieldName);
    explicit SqlBoolArr(std::vector<bool> const &Value, std::string const &FieldName = "");
    ~SqlBoolArr() override;
    SqlBoolArr(SqlBoolArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlBoolArr &operator=(SqlBoolArr const &rhs);

    /**
     * @brief value The internally stored C++ boolean array.
     */
    std::vector<bool> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<bool> *ValuePtr();

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
    explicit SqlIntArr(std::string const &FieldName);
    explicit SqlIntArr(std::vector<int32_t> const &Value, std::string const &FieldName = "");
    ~SqlIntArr() override;
    SqlIntArr(SqlIntArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlIntArr &operator=(SqlIntArr const &rhs);

    /**
     * @brief value The internally stored C++ 32-bit integer array.
     */
    std::vector<int32_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<int32_t> *ValuePtr();

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
    explicit SqlLongArr(std::string const &FieldName);
    explicit SqlLongArr(std::vector<int64_t> const &Value, std::string const &FieldName = "");
    ~SqlLongArr() override;
    SqlLongArr(SqlLongArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlLongArr &operator=(SqlLongArr const &rhs);

    /**
     * @brief value The internally stored C++ 64-bit integer array.
     */
    std::vector<int64_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<int64_t> *ValuePtr();

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
    explicit SqlFloatArr(std::string const &FieldName);
    explicit SqlFloatArr(std::vector<float> const &Value, std::string const &FieldName = "");
    ~SqlFloatArr() override;
    SqlFloatArr(SqlFloatArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlFloatArr &operator=(SqlFloatArr const &rhs);

    /**
     * @brief value The internally stored C++ single-precision float array.
     */
    std::vector<float> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<float> *ValuePtr();

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
    explicit SqlDoubleArr(std::string const &FieldName);
    explicit SqlDoubleArr(std::vector<double> const &Value, std::string const &FieldName = "");
    ~SqlDoubleArr() override;
    SqlDoubleArr(SqlDoubleArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlDoubleArr &operator=(SqlDoubleArr const &rhs);

    /**
     * @brief value The internally stored C++ double-precision float array.
     */
    std::vector<double> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<double> *ValuePtr();

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
    explicit SqlStrArr(std::string const &FieldName);
    explicit SqlStrArr(std::vector<std::string> const &Value, std::string const &FieldName = "");
    ~SqlStrArr() override;
    SqlStrArr(SqlStrArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlStrArr &operator=(SqlStrArr const &rhs);

    /**
     * @brief value The internally stored C++ string array.
     */
    std::vector<std::string> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<std::string> *ValuePtr();

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
    explicit SqlTimestampArr(std::string const &FieldName);
    explicit SqlTimestampArr(std::vector<std::time_t> const &Value,
                             std::string const &FieldName = "");
    ~SqlTimestampArr() override;
    SqlTimestampArr(SqlTimestampArr const &) = default;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlTimestampArr &operator=(SqlTimestampArr const &rhs);

    /**
     * @brief value The internally stored C++ timestamp array.
     */
    std::vector<std::time_t> const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::vector<std::time_t> *ValuePtr();

  private:
    std::vector<std::time_t> value_;
};

/**
 * @brief The SqlByteArr class byte array primitive type.
 * Note that, this array type is not nullable, so are the array elements.
 *
 * Example usage:
 * SqlByteArr rsa_key = SqlByteArr("rsa_key");
 */
class SqlByteArr : public SqlPrimitiveInterface {
  public:
    explicit SqlByteArr(std::string const &FieldName);
    explicit SqlByteArr(std::string const &Value, std::string const &FieldName);
    SqlByteArr(SqlByteArr const &) = default;
    ~SqlByteArr() override;

    void ExportToInvocation(pqxx::prepare::invocation *invocation) const override;
    void ImportFromField(pqxx::field const &field) override;

    SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) override;
    bool operator==(SqlPrimitiveInterface const &rhs) const override;
    bool operator!=(SqlPrimitiveInterface const &rhs) const override;
    bool operator<(SqlPrimitiveInterface const &rhs) const override;

    SqlByteArr &operator=(SqlByteArr const &rhs);

    /**
     * @brief value The internally stored C++ byte array.
     */
    std::string const &Value() const;

    /**
     * @brief value_ptr Mutable version of the above function.
     */
    std::string *ValuePtr();

  private:
    std::string value_;
};

} // namespace e8

#endif // SQL_PRIMITIVES_H
