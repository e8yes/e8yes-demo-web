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

/** Provides file and directory manipulation. */
public interface FileSystemProviderInterface {

  /**
   * For internal initialization.
   *
   * @throws Exception
   */
  public void init() throws Exception;

  /**
   * Clean up internal states.
   *
   * @throws Exception
   */
  public void cleanUp() throws Exception;

  /**
   * Creates and overrides a file if the the path has already existed.
   *
   * @param location File location to be created and overridden.
   * @throws IOException
   */
  public void createAndOverride(FileAccessLocation location) throws IOException;

  /**
   * Opens the file in the specified location.
   *
   * @param location Location of the file to open.
   * @return The file handle pointing to the opened file.
   * @throws IOException
   */
  public FileHandleInterface open(FileAccessLocation location) throws IOException;

  /**
   * Closes a file handle. The file handle can be closed only once.
   *
   * @param handle File handle to be closed.
   * @throws IOException
   */
  public void close(FileHandleInterface handle) throws IOException;
}
