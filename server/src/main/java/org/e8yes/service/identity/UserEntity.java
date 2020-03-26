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

import org.e8yes.sql.primitive.SqlDate;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;
import org.e8yes.sql.primitive.SqlStrArr;

/** Java class representation of the database table "auser" */
public class UserEntity {
  public SqlLong id = new SqlLong();
  public SqlStr id_str = new SqlStr();
  public SqlStrArr emails = new SqlStrArr();
  public SqlStr alias = new SqlStr();
  public SqlStr avatar_path = new SqlStr();
  public SqlStr security_key_hash = new SqlStr();
  public SqlStrArr group_names = new SqlStrArr();
  public SqlInt active_level = new SqlInt();
  public SqlDate created_at = new SqlDate();

  @Override
  public int hashCode() {
    return id.hashCode();
  }

  @Override
  public boolean equals(Object obj) {
    final UserEntity other = (UserEntity) obj;
    return id.equals(other.id);
  }

  @Override
  public String toString() {
    return "UserEntity{"
        + "id="
        + id
        + ", id_str="
        + id_str
        + ", emails="
        + emails
        + ", alias="
        + alias
        + ", avatar_path="
        + avatar_path
        + ", security_key_hash="
        + security_key_hash
        + ", group_names="
        + group_names
        + ", active_level="
        + active_level
        + ", created_at="
        + created_at
        + '}';
  }
}
