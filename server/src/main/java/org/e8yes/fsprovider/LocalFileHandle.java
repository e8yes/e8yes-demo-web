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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/** Represents handle to a file in the local file system. */
public class LocalFileHandle implements FileHandleInterface {

  public File file;
  public FileInputStream inputStream;
  public FileOutputStream outputStream;

  public LocalFileHandle(File file) {
    this.file = file;
  }

  /**
   * Reads the next specified number of bytes. If there isn't enough bytes in the file, it reads
   * whatever that is available.
   *
   * @param result A memory buffer that holds the specified number of bytes.
   * @return The number of bytes read.
   * @throws FileNotFoundException
   * @throws IOException
   */
  public int readNext(byte[] result) throws FileNotFoundException, IOException {
    if (inputStream == null) {
      inputStream = new FileInputStream(file);
    }
    return inputStream.read(result);
  }

  /**
   * Writes the next specified number of bytes to the end of the file.
   *
   * @param result The data to write to the file.
   * @throws FileNotFoundException
   * @throws IOException
   */
  public void writeNext(byte[] result) throws FileNotFoundException, IOException {
    if (outputStream == null) {
      outputStream = new FileOutputStream(file);
    }
    outputStream.write(result);
  }

  /**
   * Closing the file handle.
   *
   * @throws IOException
   */
  public void close() throws IOException {
    if (inputStream != null) {
      inputStream.close();
      inputStream = null;
    }
    if (outputStream != null) {
      outputStream.close();
      outputStream = null;
    }
  }
}
