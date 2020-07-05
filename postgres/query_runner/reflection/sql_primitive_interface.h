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

#ifndef SQL_PRIMITIVE_INTERFACE_H
#define SQL_PRIMITIVE_INTERFACE_H

#include <pqxx/pqxx>
#include <string>

namespace e8 {

/**
 * @brief The SqlPrimitiveInterface class Represents SQL primitive type and serves as a bridge
 * between SQL object and C++ object.
 */
class SqlPrimitiveInterface {
  public:
    /**
     * @brief SqlPrimitiveInterface Construct an SQL primitive field with field name reflection.
     * @param field_name The name of this primitive field.
     */
    SqlPrimitiveInterface(std::string const &FieldName);
    virtual ~SqlPrimitiveInterface();

    SqlPrimitiveInterface(SqlPrimitiveInterface const &) = default;

    /**
     * @brief field_name The name of this primitive field for reflection use.
     * @return The field name.
     */
    std::string const &FieldName() const;

    /**
     * Export value to an SQL invocatin.
     *
     * @param invocatin SQL invocation to export to.
     */
    virtual void ExportToInvocation(pqxx::prepare::invocation *invocation) const = 0;

    /**
     * Import from an SQL field and internally converts to a C++ value.
     *
     * @param field SQL field to import from.
     */
    virtual void ImportFromField(pqxx::field const &field) = 0;

    /**
     * Implementation of this operator is required.
     *
     * @param rhs Must of the same type.
     * @return This object after assignment.
     */
    virtual SqlPrimitiveInterface &operator=(SqlPrimitiveInterface const &rhs) = 0;

    /**
     * Implementation of this operator is required.
     *
     * @param rhs Must of the same type.
     * @return True if equivalent.
     */
    virtual bool operator==(SqlPrimitiveInterface const &rhs) const = 0;

    /**
     * Implementation of this operator is required.
     *
     * @param rhs Must of the same type.
     * @return True if not equivalent.
     */
    virtual bool operator!=(SqlPrimitiveInterface const &rhs) const = 0;

    /**
     * Implementation of this operator is required.
     *
     * @param rhs Must of the same type.
     * @return True if it's less than the rhs.
     */
    virtual bool operator<(SqlPrimitiveInterface const &rhs) const = 0;

  private:
    std::string field_name_;
};

} // namespace e8

#endif // SQL_PRIMITIVE_INTERFACE_H
