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

#ifndef PARSE_FLAGS_H
#define PARSE_FLAGS_H

#include <cassert>
#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>

namespace e8 {
namespace flags_internal {

std::optional<std::string> ScanFlag(std::string const &flag);

} // namespace flags_internal

template <typename FlagType> FlagType FromString(std::string const &flag_value) {
    if constexpr (std::is_same<FlagType, std::string>::value) {
        return flag_value;
    } else if constexpr (std::is_same<FlagType, int32_t>::value) {
        return std::stoi(flag_value);
    } else if constexpr (std::is_same<FlagType, uint32_t>::value) {
        return std::stoul(flag_value);
    } else if constexpr (std::is_same<FlagType, int64_t>::value) {
        return std::stoll(flag_value);
    } else if constexpr (std::is_same<FlagType, uint64_t>::value) {
        return std::stoull(flag_value);
    } else if constexpr (std::is_same<FlagType, bool>::value) {
        return flag_value == "true";
    } else if constexpr (std::is_same<FlagType, float>::value) {
        return std::stof(flag_value);
    } else if constexpr (std::is_same<FlagType, double>::value) {
        return std::stod(flag_value);
    } else {
        assert(false);
        return FlagType();
    }
}

/**
 * @brief Argv Initialize the command line arg vector before running the ReadFlag() below.
 * @param argc The number of arguments.
 * @param argv The argument vector.
 */
void Argv(int argc, char **argv);

/**
 * @brief ReadFlag Read flag value from the argument vector set by Argv();
 * @param flag_name Key of the flag value. If there are multiple argument that has the same key, the
 * first argument will be picked.
 * @param default_flag_value Default value to return from this function if the flag key isn't
 * present in the argument vector.
 * @param from_string_op A function to convert string flag value to the target type.
 * @return The converted flag value.
 */
template <typename FlagType, typename FromStringOp>
FlagType ReadFlag(std::string const &flag_name, FlagType const &default_flag_value,
                  FromStringOp from_string_op) {
    std::optional<std::string> flag_value = flags_internal::ScanFlag(flag_name);
    if (!flag_value.has_value()) {
        return default_flag_value;
    }
    return from_string_op(*flag_value);
}

} // namespace e8

#endif // PARSE_FLAGS_H
