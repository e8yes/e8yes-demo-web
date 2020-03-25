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

import org.e8yes.service.StorageVolume;

/** Represents all the information to specified the location of a file. */
public class FileAccessLocation {
  public StorageVolume vol;
  public String path;

  public FileAccessLocation(StorageVolume vol, String path) {
    this.vol = vol;
    this.path = path;
  }

  @Override
  public int hashCode() {
    int hash = 7;
    hash = hash * 23 + vol.hashCode();
    hash = hash * 23 + path.hashCode();
    return hash;
  }

  @Override
  public boolean equals(Object obj) {
    final FileAccessLocation other = (FileAccessLocation) obj;
    return this.vol == other.vol && this.path.equals(other.path);
  }

  @Override
  public String toString() {
    return "FileAccessLocation{" + "vol=" + vol + ", path=" + path + '}';
  }
}
