/**
 * e8yes demo web.
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

#ifndef FILESYSTEMINTERFACE_H
#define FILESYSTEMINTERFACE_H

#include <memory>
#include <string>

#include "file_system/file_interface.h"

namespace e8 {

/**
 * @brief The FileSystemInterface class
 */
class FileSystemInterface {
  public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    /**
     * Creates an empty file at the specified file path. It overrides the file at that location if
     * the the file path has already existed.
     *
     * @return true if no error occurred.
     */
    virtual bool CreateFile(std::string const &file_path) = 0;

    /**
     * Opens the file at the specified location.
     *
     * @param file_path Location of the file to open.
     * @return The file handle pointing to the opened file.
     */
    virtual std::unique_ptr<FileInterface> OpenFile(std::string const &file_path) = 0;
};

} // namespace e8

#endif // FILESYSTEMINTERFACE_H
