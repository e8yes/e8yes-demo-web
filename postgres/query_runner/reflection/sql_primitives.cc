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

#include <cassert>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <pqxx/pqxx>
#include <stdint.h>
#include <string>

#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {
namespace {

#include <iostream>

void from_string(std::string const &str_val, bool *val) {
    if (str_val == "t") {
        *val = true;
    } else {
        *val = false;
    }
}
void from_string(std::string const &str_val, int32_t *val) { *val = std::stoi(str_val); }
void from_string(std::string const &str_val, int64_t *val) { *val = std::stol(str_val); }
void from_string(std::string const &str_val, float *val) { *val = std::stof(str_val); }
void from_string(std::string const &str_val, double *val) { *val = std::stod(str_val); }
void from_string(std::string const &str_val, std::string *val) { *val = str_val; }

std::time_t timestamp_from_string(std::string const &str_val) {
    std::istringstream in(str_val);
    std::tm t{};
    in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    assert(!in.fail());
    std::time_t local_timestamp = std::mktime(&t);
    std::time_t utc_timestamp = local_timestamp + t.tm_gmtoff;
    return utc_timestamp;
}

std::string timestamp_to_string(std::time_t const &timestamp) {
    std::ostringstream out;
    std::tm const *t = std::gmtime(&timestamp);
    out << std::put_time(t, "%Y-%m-%d %H:%M:%S");
    assert(!out.fail());
    return out.str();
}

template <typename ElementType, bool timestamp = false>
void ReadArray(pqxx::array_parser *src, std::vector<ElementType> *dst) {
    while (true) {
        auto [juncture, element] = src->get_next();
        switch (juncture) {
        case pqxx::array_parser::juncture::string_value: {
            ElementType element_val;
            if constexpr (timestamp) {
                element_val = timestamp_from_string(element);
            } else {
                from_string(element, &element_val);
            }
            dst->push_back(element_val);
            break;
        }
        case pqxx::array_parser::juncture::done:
            return;
        default:
            // This will skip null value because this primitive type does not store null array
            // elements.
            continue;
        }
    }
}

template <typename ElementType, bool timestamp = false>
std::string ArrayToPsqlString(std::vector<ElementType> const &arr) {
    std::string psql_str = "{";
    for (unsigned i = 0; i < arr.size(); i++) {
        psql_str += "\"";
        if constexpr (std::is_same<ElementType, std::string>::value) {
            psql_str += arr[i];
        } else if constexpr (timestamp) {
            psql_str += timestamp_to_string(arr[i]);
        } else {
            psql_str += std::to_string(arr[i]);
        }
        psql_str += "\"";
        if (i != arr.size() - 1) {
            psql_str += ",";
        }
    }
    psql_str += "}";
    return psql_str;
}

} // namespace

SqlBool::SqlBool(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlBool::SqlBool(bool value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlBool::~SqlBool() {}

void SqlBool::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlBool::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<bool>();
    }
}

SqlBool &SqlBool::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlBool const &>(rhs).value_;
    return *this;
}

bool SqlBool::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlBool const &other = static_cast<SqlBool const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() == other.value_.value();
}

bool SqlBool::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlBool::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlBool const &other = static_cast<SqlBool const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<bool> const &SqlBool::value() const { return value_; }

std::optional<bool> *SqlBool::value_ptr() { return &value_; }

SqlInt::SqlInt(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlInt::SqlInt(int32_t value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlInt::~SqlInt() {}

void SqlInt::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlInt::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<int32_t>();
    }
}

SqlInt &SqlInt::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlInt const &>(rhs).value_;
    return *this;
}

bool SqlInt::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlInt const &other = static_cast<SqlInt const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() == other.value_.value();
}

bool SqlInt::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlInt::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlInt const &other = static_cast<SqlInt const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<int32_t> const &SqlInt::value() const { return value_; }

std::optional<int32_t> *SqlInt::value_ptr() { return &value_; }

SqlLong::SqlLong(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlLong::SqlLong(int64_t value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlLong::~SqlLong() {}

void SqlLong::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlLong::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<int64_t>();
    }
}

SqlLong &SqlLong::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlLong const &>(rhs).value_;
    return *this;
}

bool SqlLong::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlLong const &other = static_cast<SqlLong const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() == other.value_.value();
}

bool SqlLong::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlLong::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlLong const &other = static_cast<SqlLong const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<int64_t> const &SqlLong::value() const { return value_; }

std::optional<int64_t> *SqlLong::value_ptr() { return &value_; }

SqlFloat::SqlFloat(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlFloat::SqlFloat(float value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlFloat::~SqlFloat() {}

void SqlFloat::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlFloat::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<float>();
    }
}

SqlFloat &SqlFloat::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlFloat const &>(rhs).value_;
    return *this;
}

bool SqlFloat::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlFloat const &other = static_cast<SqlFloat const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return std::abs(value_.value() - other.value_.value()) < 1e-4f;
}

bool SqlFloat::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlFloat::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlFloat const &other = static_cast<SqlFloat const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<float> const &SqlFloat::value() const { return value_; }

std::optional<float> *SqlFloat::value_ptr() { return &value_; }

SqlDouble::SqlDouble(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlDouble::SqlDouble(double value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlDouble::~SqlDouble() {}

void SqlDouble::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlDouble::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<double>();
    }
}

SqlDouble &SqlDouble::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlDouble const &>(rhs).value_;
    return *this;
}

bool SqlDouble::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlDouble const &other = static_cast<SqlDouble const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return std::abs(value_.value() - other.value_.value()) < 1e-4;
}

bool SqlDouble::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlDouble::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlDouble const &other = static_cast<SqlDouble const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<double> const &SqlDouble::value() const { return value_; }

std::optional<double> *SqlDouble::value_ptr() { return &value_; }

SqlStr::SqlStr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlStr::SqlStr(std::string const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlStr::~SqlStr() {}

void SqlStr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    (*invocation)(value_.value());
}

void SqlStr::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<std::string>();
    }
}

SqlStr &SqlStr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlStr const &>(rhs).value_;
    return *this;
}

bool SqlStr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlStr const &other = static_cast<SqlStr const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() == other.value_.value();
}

bool SqlStr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlStr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlStr const &other = static_cast<SqlStr const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<std::string> const &SqlStr::value() const { return value_; }

std::optional<std::string> *SqlStr::value_ptr() { return &value_; }

SqlTimestamp::SqlTimestamp(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlTimestamp::SqlTimestamp(std::time_t value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlTimestamp::~SqlTimestamp() {}

void SqlTimestamp::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    assert(value_.has_value());
    std::string timestamp_str = timestamp_to_string(value_.value());
    (*invocation)(timestamp_str);
}

void SqlTimestamp::import_from_field(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        std::string timestamp_str = field.c_str();
        value_ = timestamp_from_string(timestamp_str);
    }
}

SqlTimestamp &SqlTimestamp::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlTimestamp const &>(rhs).value_;
    return *this;
}

bool SqlTimestamp::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlTimestamp const &other = static_cast<SqlTimestamp const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() == other.value_.value();
}

bool SqlTimestamp::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlTimestamp::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlTimestamp const &other = static_cast<SqlTimestamp const &>(rhs);
    assert(value_.has_value());
    assert(other.value_.has_value());
    return value_.value() < other.value_.value();
}

std::optional<std::time_t> const &SqlTimestamp::value() const { return value_; }

std::optional<std::time_t> *SqlTimestamp::value_ptr() { return &value_; }

SqlBoolArr::SqlBoolArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlBoolArr::SqlBoolArr(std::vector<bool> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlBoolArr::~SqlBoolArr() {}

void SqlBoolArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlBoolArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlBoolArr &SqlBoolArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlBoolArr const &>(rhs).value_;
    return *this;
}

bool SqlBoolArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlBoolArr const &other = static_cast<SqlBoolArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlBoolArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlBoolArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlBoolArr const &other = static_cast<SqlBoolArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<bool> const &SqlBoolArr::value() const { return value_; }

std::vector<bool> *SqlBoolArr::value_ptr() { return &value_; }

SqlIntArr::SqlIntArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlIntArr::SqlIntArr(std::vector<int32_t> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlIntArr::~SqlIntArr() {}

void SqlIntArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlIntArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlIntArr &SqlIntArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlIntArr const &>(rhs).value_;
    return *this;
}

bool SqlIntArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlIntArr const &other = static_cast<SqlIntArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlIntArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlIntArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlIntArr const &other = static_cast<SqlIntArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<int32_t> const &SqlIntArr::value() const { return value_; }

std::vector<int32_t> *SqlIntArr::value_ptr() { return &value_; }

SqlLongArr::SqlLongArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlLongArr::SqlLongArr(std::vector<int64_t> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlLongArr::~SqlLongArr() {}

void SqlLongArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlLongArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlLongArr &SqlLongArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlLongArr const &>(rhs).value_;
    return *this;
}

bool SqlLongArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlLongArr const &other = static_cast<SqlLongArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlLongArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlLongArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlLongArr const &other = static_cast<SqlLongArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<int64_t> const &SqlLongArr::value() const { return value_; }

std::vector<int64_t> *SqlLongArr::value_ptr() { return &value_; }

SqlFloatArr::SqlFloatArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlFloatArr::SqlFloatArr(std::vector<float> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlFloatArr::~SqlFloatArr() {}

void SqlFloatArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlFloatArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlFloatArr &SqlFloatArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlFloatArr const &>(rhs).value_;
    return *this;
}

bool SqlFloatArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlFloatArr const &other = static_cast<SqlFloatArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlFloatArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlFloatArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlFloatArr const &other = static_cast<SqlFloatArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<float> const &SqlFloatArr::value() const { return value_; }

std::vector<float> *SqlFloatArr::value_ptr() { return &value_; }

SqlDoubleArr::SqlDoubleArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlDoubleArr::SqlDoubleArr(std::vector<double> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlDoubleArr::~SqlDoubleArr() {}

void SqlDoubleArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlDoubleArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlDoubleArr &SqlDoubleArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlDoubleArr const &>(rhs).value_;
    return *this;
}

bool SqlDoubleArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlDoubleArr const &other = static_cast<SqlDoubleArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlDoubleArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlDoubleArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlDoubleArr const &other = static_cast<SqlDoubleArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<double> const &SqlDoubleArr::value() const { return value_; }

std::vector<double> *SqlDoubleArr::value_ptr() { return &value_; }

SqlStrArr::SqlStrArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlStrArr::SqlStrArr(std::vector<std::string> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlStrArr::~SqlStrArr() {}

void SqlStrArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlStrArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlStrArr &SqlStrArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlStrArr const &>(rhs).value_;
    return *this;
}

bool SqlStrArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlStrArr const &other = static_cast<SqlStrArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlStrArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlStrArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlStrArr const &other = static_cast<SqlStrArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<std::string> const &SqlStrArr::value() const { return value_; }

std::vector<std::string> *SqlStrArr::value_ptr() { return &value_; }

SqlTimestampArr::SqlTimestampArr(std::string const &field_name)
    : SqlPrimitiveInterface(field_name) {}

SqlTimestampArr::SqlTimestampArr(std::vector<std::time_t> const &value,
                                 std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlTimestampArr::~SqlTimestampArr() {}

void SqlTimestampArr::export_to_invocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString<std::time_t, /*timestamp=*/true>(value_));
}

void SqlTimestampArr::import_from_field(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray<std::time_t, /*timestamp=*/true>(&arr_parser, &value_);
    }
}

SqlTimestampArr &SqlTimestampArr::operator=(SqlPrimitiveInterface const &rhs) {
    value_ = static_cast<SqlTimestampArr const &>(rhs).value_;
    return *this;
}

bool SqlTimestampArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlTimestampArr const &other = static_cast<SqlTimestampArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlTimestampArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlTimestampArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlTimestampArr const &other = static_cast<SqlTimestampArr const &>(rhs);
    return value_ < other.value_;
}

std::vector<std::time_t> const &SqlTimestampArr::value() const { return value_; }

std::vector<std::time_t> *SqlTimestampArr::value_ptr() { return &value_; }

} // namespace e8
