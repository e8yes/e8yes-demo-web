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
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

/** Represents handle to a file in the local file system. */
public class LocalFileHandle implements FileHandleInterface {

  private final FileChannel file;
  private final FileLock lock;

  public LocalFileHandle(Path path) throws IOException {
    file =
        FileChannel.open(
            path, StandardOpenOption.READ, StandardOpenOption.WRITE, StandardOpenOption.CREATE);
    lock = file.lock();
  }

  @Override
  public int readNext(ByteBuffer result) throws IOException {
    return file.read(result);
  }

  @Override
  public int writeNext(ByteBuffer data) throws IOException {
    return file.write(data);
  }

  @Override
  public void close() throws IOException {
    lock.release();
    lock.close();
    file.close();
  }
}
