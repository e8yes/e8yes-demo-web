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
package org.e8yes.fsprovider;

import java.io.IOException;
import java.nio.ByteBuffer;

/** Represents a handle(pointer) to a file. */
public interface FileHandleInterface {

  /**
   * Reads the next specified number of bytes. If the remaining file is shorter than the number of
   * bytes requested, it fills an array with the number of bytes available.
   *
   * @param result A memory buffer which holds the specified number bytes that can be read.
   * @return The number of bytes read.
   * @throws IOException
   */
  public int readNext(ByteBuffer result) throws IOException;

  /**
   * Writes the next specified number of bytes to the end of the file.
   *
   * @param data The data to write to the file.
   * @return The number of bytes written.
   * @throws IOException
   */
  public int writeNext(ByteBuffer data) throws IOException;

  /**
   * Closing the file handle.
   *
   * @throws IOException
   */
  public void close() throws IOException;
}
