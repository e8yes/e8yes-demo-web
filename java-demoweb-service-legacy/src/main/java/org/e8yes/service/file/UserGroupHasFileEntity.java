/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, b ut WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.service.file;

import org.e8yes.sql.primitive.SqlBool;
import org.e8yes.sql.primitive.SqlStr;

/** Java class representation of the database table "user_group_has_file" */
public class UserGroupHasFileEntity {
  public SqlStr group_name = new SqlStr();
  public SqlStr file_path = new SqlStr();
  public SqlBool can_read = new SqlBool();
  public SqlBool can_write = new SqlBool();

  @Override
  public int hashCode() {
    int hash = 7;
    hash = hash * 23 + group_name.hashCode();
    hash = hash * 23 + file_path.hashCode();
    return hash;
  }

  @Override
  public boolean equals(Object obj) {
    final UserGroupHasFileEntity other = (UserGroupHasFileEntity) obj;
    return group_name.equals(other.group_name) && file_path.equals(other.file_path);
  }

  @Override
  public String toString() {
    return "UserGroupHasFileEntity{"
        + "group_name="
        + group_name
        + ", file_path="
        + file_path
        + ", can_read="
        + can_read
        + ", can_write="
        + can_write
        + '}';
  }
}
