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
package org.e8yes.environment;

public class DatabaseInfo {
  public String hostName;
  public int port;
  public String dbName;
  public String userName;
  public String password;

  public DatabaseInfo(String hostName, int port, String dbName, String userName, String password) {
    this.hostName = hostName;
    this.port = port;
    this.dbName = dbName;
    this.userName = userName;
    this.password = password;
  }

  public static DatabaseInfo get(EnvironmentContext ctx) {
    switch (ctx.mode) {
      case Prod:
        return new DatabaseInfo(
            /*hostName*/ "localhost",
            /*port=*/ 5432,
            /*dbName=*/ "demoweb",
            /*userName=*/ "postgres",
            /*password=*/ "password");
      case Test:
        return new DatabaseInfo(
            /*hostName*/ "localhost",
            /*port=*/ 5432,
            /*dbName=*/ "demowebtest",
            /*userName=*/ "postgres",
            /*password=*/ "password");
      default:
        throw new IllegalArgumentException("Unknown environment context mode=" + ctx.mode);
    }
  }
}
