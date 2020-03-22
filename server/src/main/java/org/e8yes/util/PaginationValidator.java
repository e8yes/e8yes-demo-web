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
package org.e8yes.util;

import org.e8yes.service.Pagination;

/** Validate the value of a pagination object. */
public class PaginationValidator {

  public static void validate(Pagination pagination, int resultPerPageLimit)
      throws IllegalArgumentException {
    if (pagination.getPageNumber() < 0) {
      throw new IllegalArgumentException(
          "pageNumber must be >= 0, but got=" + pagination.getPageNumber());
    }
    if (pagination.getResultPerPage() < 0) {
      throw new IllegalArgumentException(
          "resultPerPage must be >= 0, but got=" + pagination.getResultPerPage());
    }
    if (pagination.getResultPerPage() > resultPerPageLimit) {
      throw new IllegalArgumentException(
          "resultPerPageLimit is "
              + resultPerPageLimit
              + ", but got="
              + pagination.getResultPerPage());
    }
  }
}
