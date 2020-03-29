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
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

/** Provides means of local file system manipulation. */
public class LocalFileSystemProvider implements FileSystemProviderInterface {

  private final Path rootPath;

  public LocalFileSystemProvider(String rootPath) {
    this.rootPath = Path.of(rootPath);
  }

  @Override
  public void init() throws Exception {
    this.rootPath.toFile().mkdirs();
  }

  @Override
  public void cleanUp() throws Exception {}

  @Override
  public void createAndOverride(FileAccessLocation location) throws IOException {
    Path path = Path.of(rootPath.toString(), location.path);

    Path dir = path.getParent();
    if (!Files.exists(dir)) {
      dir.toFile().mkdirs();
    }

    path.toFile().delete();
    path.toFile().createNewFile();
  }

  @Override
  public FileHandleInterface open(FileAccessLocation location) throws IOException {
    Path path = Path.of(rootPath.toString(), location.path);
    File file = path.toFile();
    assert (file.exists());
    return new LocalFileHandle(file);
  }

  @Override
  public int read(FileHandleInterface handle, byte[] result) throws IOException {
    assert (handle instanceof LocalFileHandle);
    LocalFileHandle localFile = (LocalFileHandle) handle;
    assert (localFile.file.canRead());
    return localFile.readNext(result);
  }

  @Override
  public void write(FileHandleInterface handle, byte[] data) throws IOException {
    assert (handle instanceof LocalFileHandle);
    LocalFileHandle localFile = (LocalFileHandle) handle;
    assert (localFile.file.canWrite());
    localFile.writeNext(data);
  }

  @Override
  public void close(FileHandleInterface handle) throws IOException {
    assert (handle instanceof LocalFileHandle);
    LocalFileHandle localFile = (LocalFileHandle) handle;
    localFile.close();
  }
}
