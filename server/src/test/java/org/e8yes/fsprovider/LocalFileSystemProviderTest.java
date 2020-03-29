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

import java.io.IOException;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class LocalFileSystemProviderTest {

  @Test
  public void fileIOTest() throws IOException, Exception {
    LocalFileSystemProvider localFs = new LocalFileSystemProvider(/*rootPath=*/ "./demowebtestfs");
    localFs.init();

    FileAccessLocation location = new FileAccessLocation("/test_dir/test_file");
    localFs.createAndOverride(location);

    FileHandleInterface handle = localFs.open(location);
    byte[] data = new byte[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    localFs.write(handle, data);
    byte[] extraByte = new byte[] {10};
    localFs.write(handle, extraByte);
    localFs.close(handle);

    handle = localFs.open(location);
    byte[] buf = new byte[5];
    int numBytesRead = localFs.read(handle, buf);
    Assertions.assertEquals(5, numBytesRead);
    Assertions.assertArrayEquals(new byte[] {0, 1, 2, 3, 4}, buf);

    numBytesRead = localFs.read(handle, buf);
    Assertions.assertEquals(5, numBytesRead);
    Assertions.assertArrayEquals(new byte[] {5, 6, 7, 8, 9}, buf);

    numBytesRead = localFs.read(handle, buf);
    Assertions.assertEquals(1, numBytesRead);
    Assertions.assertArrayEquals(new byte[] {10, 6, 7, 8, 9}, buf);

    localFs.close(handle);
  }
}
