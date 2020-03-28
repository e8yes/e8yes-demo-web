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
package org.e8yes.service.identity;

import org.e8yes.sql.primitive.SqlIntArr;
import org.e8yes.sql.primitive.SqlStr;

/** Java class representation of the database table "user_group" */
public class UserGroupEntity {
  public SqlStr group_name = new SqlStr();
  public SqlIntArr permissions = new SqlIntArr();

  @Override
  public int hashCode() {
    return group_name.hashCode();
  }

  @Override
  public boolean equals(Object obj) {
    final UserGroupEntity other = (UserGroupEntity) obj;
    return this.group_name.equals(other.group_name);
  }

  @Override
  public String toString() {
    return "UserGroupEntity{" + "group_name=" + group_name + ", permissions=" + permissions + '}';
  }
}
