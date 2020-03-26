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

import org.e8yes.sql.primitive.SqlDate;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlLong;
import org.e8yes.sql.primitive.SqlStr;

/** Java class representation of the database table "file" */
public class FileEntity {
  public SqlStr path = new SqlStr();
  public SqlInt format = new SqlInt();
  public SqlInt encryption_key_source = new SqlInt();
  public SqlLong storage_size = new SqlLong();
  public SqlDate created_at = new SqlDate();
  public SqlDate last_modified_at = new SqlDate();

  @Override
  public int hashCode() {
    int hash = 7;
    hash = hash * 23 + path.hashCode();
    return hash;
  }

  @Override
  public boolean equals(Object obj) {
    final FileEntity other = (FileEntity) obj;
    return path.equals(other.path);
  }

  @Override
  public String toString() {
    return "FileEntity{"
        + "path="
        + path
        + ", format="
        + format
        + ", encryption_key_source="
        + encryption_key_source
        + ", storage_size="
        + storage_size
        + ", created_at="
        + created_at
        + ", last_modified_at="
        + last_modified_at
        + '}';
  }
}
