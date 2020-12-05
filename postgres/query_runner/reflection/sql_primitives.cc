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
#include <iomanip>
#include <pqxx/pqxx>
#include <stdint.h>
#include <string>

#include "common/time_util/time_util.h"
#include "postgres/query_runner/reflection/sql_primitives.h"

namespace e8 {
namespace {

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

std::vector<std::string> split(std::string const &input, char const delim) {
    std::vector<std::string> elems;

    std::stringstream ss(input);
    std::string piece;
    while (std::getline(ss, piece, delim)) {
        elems.push_back(piece);
    }

    return elems;
}

TimestampMicros timestamp_from_string(std::string const &str_val) {
    std::vector<std::string> pieces = split(str_val, '.');
    assert(pieces.size() == 1 || pieces.size() == 2);

    std::string date = pieces[0];

    std::istringstream in(date);
    std::tm t{};
    in >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    assert(!in.fail());

    std::time_t local_timestamp = std::mktime(&t);
    TimestampMicros utc_timestamp = (local_timestamp + t.tm_gmtoff) * 1000000;

    if (pieces.size() == 2) {
        std::string micros = pieces[1];
        utc_timestamp += std::stol(micros);
    }

    return utc_timestamp;
}

std::string timestamp_to_string(TimestampMicros const &timestamp) {
    TimestampSecs epoch_secs = timestamp / 1000000;
    TimestampMicros micros_remainder = timestamp % 1000000;

    std::ostringstream out;
    std::tm const *t = std::gmtime(&epoch_secs);
    out << std::put_time(t, "%Y-%m-%d %H:%M:%S") << '.' << micros_remainder;
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

void SqlBool::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(bool(), false);
    }
}

void SqlBool::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<bool>();
    }
}

SqlPrimitiveInterface &SqlBool::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlBool const &>(rhs));
}

SqlBool &SqlBool::operator=(SqlBool const &rhs) {
    value_ = rhs.value_;
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

std::optional<bool> const &SqlBool::Value() const { return value_; }

std::optional<bool> *SqlBool::ValuePtr() { return &value_; }

SqlInt::SqlInt(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlInt::SqlInt(int32_t value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlInt::~SqlInt() {}

void SqlInt::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(int32_t(), false);
    }
}

void SqlInt::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<int32_t>();
    }
}

SqlPrimitiveInterface &SqlInt::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlInt const &>(rhs));
}

SqlInt &SqlInt::operator=(SqlInt const &rhs) {
    value_ = rhs.value_;
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

std::optional<int32_t> const &SqlInt::Value() const { return value_; }

std::optional<int32_t> *SqlInt::ValuePtr() { return &value_; }

SqlLong::SqlLong(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlLong::SqlLong(int64_t value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlLong::~SqlLong() {}

void SqlLong::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(int64_t(), false);
    }
}

void SqlLong::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<int64_t>();
    }
}

SqlPrimitiveInterface &SqlLong::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlLong const &>(rhs));
}

SqlLong &SqlLong::operator=(SqlLong const &rhs) {
    value_ = rhs.value_;
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

std::optional<int64_t> const &SqlLong::Value() const { return value_; }

std::optional<int64_t> *SqlLong::ValuePtr() { return &value_; }

SqlFloat::SqlFloat(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlFloat::SqlFloat(float value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlFloat::~SqlFloat() {}

void SqlFloat::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(float(), false);
    }
}

void SqlFloat::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<float>();
    }
}

SqlPrimitiveInterface &SqlFloat::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlFloat const &>(rhs));
}

SqlFloat &SqlFloat::operator=(SqlFloat const &rhs) {
    value_ = rhs.value_;
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

std::optional<float> const &SqlFloat::Value() const { return value_; }

std::optional<float> *SqlFloat::ValuePtr() { return &value_; }

SqlDouble::SqlDouble(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlDouble::SqlDouble(double value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlDouble::~SqlDouble() {}

void SqlDouble::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(double(), false);
    }
}

void SqlDouble::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<double>();
    }
}

SqlPrimitiveInterface &SqlDouble::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlDouble const &>(rhs));
}

SqlDouble &SqlDouble::operator=(SqlDouble const &rhs) {
    value_ = rhs.value_;
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

std::optional<double> const &SqlDouble::Value() const { return value_; }

std::optional<double> *SqlDouble::ValuePtr() { return &value_; }

SqlStr::SqlStr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlStr::SqlStr(std::string const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlStr::~SqlStr() {}

void SqlStr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        (*invocation)(value_.value());
    } else {
        (*invocation)(std::string(), false);
    }
}

void SqlStr::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        value_ = field.as<std::string>();
    }
}

SqlPrimitiveInterface &SqlStr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlStr const &>(rhs));
}

SqlStr &SqlStr::operator=(SqlStr const &rhs) {
    value_ = rhs.value_;
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

std::optional<std::string> const &SqlStr::Value() const { return value_; }

std::optional<std::string> *SqlStr::ValuePtr() { return &value_; }

SqlTimestamp::SqlTimestamp(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlTimestamp::SqlTimestamp(TimestampMicros value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlTimestamp::~SqlTimestamp() {}

void SqlTimestamp::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    if (value_.has_value()) {
        std::string timestamp_str = timestamp_to_string(value_.value());
        (*invocation)(timestamp_str);
    } else {
        (*invocation)("", false);
    }
}

void SqlTimestamp::ImportFromField(pqxx::field const &field) {
    if (field.is_null()) {
        value_ = std::nullopt;
    } else {
        std::string timestamp_str = field.c_str();
        value_ = timestamp_from_string(timestamp_str);
    }
}

SqlPrimitiveInterface &SqlTimestamp::operator=(SqlPrimitiveInterface const &rhs) {
    return *this = static_cast<SqlTimestamp const &>(rhs);
}

SqlTimestamp &SqlTimestamp::operator=(SqlTimestamp const &rhs) {
    value_ = rhs.value_;
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

std::optional<TimestampMicros> const &SqlTimestamp::Value() const { return value_; }

std::optional<TimestampMicros> *SqlTimestamp::ValuePtr() { return &value_; }

SqlBoolArr::SqlBoolArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlBoolArr::SqlBoolArr(std::vector<bool> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlBoolArr::~SqlBoolArr() {}

void SqlBoolArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlBoolArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlBoolArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlBoolArr const &>(rhs));
}

SqlBoolArr &SqlBoolArr::operator=(SqlBoolArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<bool> const &SqlBoolArr::Value() const { return value_; }

std::vector<bool> *SqlBoolArr::ValuePtr() { return &value_; }

SqlIntArr::SqlIntArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlIntArr::SqlIntArr(std::vector<int32_t> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlIntArr::~SqlIntArr() {}

void SqlIntArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlIntArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlIntArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlIntArr const &>(rhs));
}

SqlIntArr &SqlIntArr::operator=(SqlIntArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<int32_t> const &SqlIntArr::Value() const { return value_; }

std::vector<int32_t> *SqlIntArr::ValuePtr() { return &value_; }

SqlLongArr::SqlLongArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlLongArr::SqlLongArr(std::vector<int64_t> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlLongArr::~SqlLongArr() {}

void SqlLongArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlLongArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlLongArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlLongArr const &>(rhs));
}

SqlLongArr &SqlLongArr::operator=(SqlLongArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<int64_t> const &SqlLongArr::Value() const { return value_; }

std::vector<int64_t> *SqlLongArr::ValuePtr() { return &value_; }

SqlFloatArr::SqlFloatArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlFloatArr::SqlFloatArr(std::vector<float> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlFloatArr::~SqlFloatArr() {}

void SqlFloatArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlFloatArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlFloatArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlFloatArr const &>(rhs));
}

SqlFloatArr &SqlFloatArr::operator=(SqlFloatArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<float> const &SqlFloatArr::Value() const { return value_; }

std::vector<float> *SqlFloatArr::ValuePtr() { return &value_; }

SqlDoubleArr::SqlDoubleArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlDoubleArr::SqlDoubleArr(std::vector<double> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlDoubleArr::~SqlDoubleArr() {}

void SqlDoubleArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlDoubleArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlDoubleArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlDoubleArr const &>(rhs));
}

SqlDoubleArr &SqlDoubleArr::operator=(SqlDoubleArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<double> const &SqlDoubleArr::Value() const { return value_; }

std::vector<double> *SqlDoubleArr::ValuePtr() { return &value_; }

SqlStrArr::SqlStrArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlStrArr::SqlStrArr(std::vector<std::string> const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlStrArr::~SqlStrArr() {}

void SqlStrArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString(value_));
}

void SqlStrArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlStrArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlStrArr const &>(rhs));
}

SqlStrArr &SqlStrArr::operator=(SqlStrArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<std::string> const &SqlStrArr::Value() const { return value_; }

std::vector<std::string> *SqlStrArr::ValuePtr() { return &value_; }

SqlTimestampArr::SqlTimestampArr(std::string const &field_name)
    : SqlPrimitiveInterface(field_name) {}

SqlTimestampArr::SqlTimestampArr(std::vector<TimestampMicros> const &value,
                                 std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlTimestampArr::~SqlTimestampArr() {}

void SqlTimestampArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    (*invocation)(ArrayToPsqlString<TimestampMicros, /*timestamp=*/true>(value_));
}

void SqlTimestampArr::ImportFromField(pqxx::field const &field) {
    value_.clear();

    if (!field.is_null()) {
        // If the field is indeed null, this primitive type will instead store an empty array.
        pqxx::array_parser arr_parser = field.as_array();
        ReadArray<TimestampMicros, /*timestamp=*/true>(&arr_parser, &value_);
    }
}

SqlPrimitiveInterface &SqlTimestampArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlTimestampArr const &>(rhs));
}

SqlTimestampArr &SqlTimestampArr::operator=(SqlTimestampArr const &rhs) {
    value_ = rhs.value_;
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

std::vector<TimestampMicros> const &SqlTimestampArr::Value() const { return value_; }

std::vector<TimestampMicros> *SqlTimestampArr::ValuePtr() { return &value_; }

SqlByteArr::SqlByteArr(std::string const &field_name) : SqlPrimitiveInterface(field_name) {}

SqlByteArr::SqlByteArr(std::string const &value, std::string const &field_name)
    : SqlPrimitiveInterface(field_name), value_(value) {}

SqlByteArr::~SqlByteArr() {}

void SqlByteArr::ExportToInvocation(pqxx::prepare::invocation *invocation) const {
    pqxx::binarystring blob(value_.data(), value_.size());
    (*invocation)(blob);
}

void SqlByteArr::ImportFromField(pqxx::field const &field) {
    pqxx::binarystring blob(field);
    uint8_t const *ptr = blob.data();
    size_t size = blob.size();

    value_.resize(size);
    for (unsigned i = 0; i < size; i++) {
        value_[i] = ptr[i];
    }
}

SqlPrimitiveInterface &SqlByteArr::operator=(SqlPrimitiveInterface const &rhs) {
    return (*this = static_cast<SqlByteArr const &>(rhs));
}

bool SqlByteArr::operator==(SqlPrimitiveInterface const &rhs) const {
    SqlByteArr const &other = static_cast<SqlByteArr const &>(rhs);
    return value_ == other.value_;
}

bool SqlByteArr::operator!=(SqlPrimitiveInterface const &rhs) const { return !(*this == rhs); }

bool SqlByteArr::operator<(SqlPrimitiveInterface const &rhs) const {
    SqlByteArr const &other = static_cast<SqlByteArr const &>(rhs);
    return value_ < other.value_;
}

SqlByteArr &SqlByteArr::operator=(SqlByteArr const &rhs) {
    value_ = rhs.value_;
    return *this;
}

std::string const &SqlByteArr::Value() const { return value_; }

std::string *SqlByteArr::ValuePtr() { return &value_; }

} // namespace e8
