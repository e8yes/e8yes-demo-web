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
package org.e8yes.fsprovider;

/** Represents all the information to specified the location of a file. */
public class FileAccessLocation {
  public String path;

  public FileAccessLocation(String path) {
    this.path = path;
  }

  @Override
  public int hashCode() {
    int hash = 7;
    hash = hash * 23 + path.hashCode();
    return hash;
  }

  @Override
  public boolean equals(Object obj) {
    final FileAccessLocation other = (FileAccessLocation) obj;
    return path.equals(other.path);
  }

  @Override
  public String toString() {
    return "FileAccessLocation{path=" + path + '}';
  }
}
