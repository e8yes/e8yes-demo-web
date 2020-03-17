/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Yichen Ma {yichenm2@uci.edu}, Chifeng Wen {daviesx66@gmail.com}
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
package org.e8yes.sql;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.List;
import org.e8yes.sql.connection.BasicConnectionReservoir;
import org.e8yes.sql.connection.ConnectionFactory;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlStr;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class SqlRunnerTest {

  public static class UserHasManyCreditCards {
    public static class User {
      public SqlInt id = new SqlInt();
      public SqlStr userName = new SqlStr();

      @Override
      public boolean equals(Object o) {
        return id.equals(((User) o).id);
      }

      @Override
      public int hashCode() {
        return id.hashCode();
      }
    }

    public static class Cards {
      public SqlInt id = new SqlInt();
      public SqlStr number = new SqlStr();
      public SqlInt userId = new SqlInt();
    }

    public User userInfo;
    public Cards[] cards;

    @Override
    public boolean equals(Object o) {
      return userInfo.equals(((UserHasManyCreditCards) o).userInfo);
    }

    @Override
    public int hashCode() {
      return userInfo.hashCode();
    }
  }

  @Test
  public void testRunUpdateAndQuery()
      throws SQLException, NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionFactory factory =
        new ConnectionFactory(
            ConnectionFactory.ConnectionType.JDBC,
            /*hostName=*/ "localhost",
            /*port=*/ 5432,
            /*dbName=*/ "demowebtest",
            /*userName=*/ "postgres",
            /*password=*/ "password");
    ConnectionInterface conn = factory.create();

    // Prepare test data.
    String dropUserTable = "DROP TABLE IF EXISTS QueryRunnerTestUser";
    String dropCardTable = "DROP TABLE IF EXISTS QueryRunnerTestCard";
    conn.runUpdate(dropCardTable, new ConnectionInterface.QueryParams());
    conn.runUpdate(dropUserTable, new ConnectionInterface.QueryParams());

    String createUserTable =
        "CREATE TABLE QueryRunnerTestUser("
            + " id INTEGER NOT NULL,"
            + " userName CHARACTER VARYING NOT NULL,"
            + " PRIMARY KEY (id))";
    conn.runUpdate(createUserTable, new ConnectionInterface.QueryParams());
    String createCardTable =
        "CREATE TABLE QueryRunnerTestCard("
            + " id INTEGER NOT NULL,"
            + " userId INTEGER NOT NULL,"
            + " number CHARACTER VARYING NOT NULL,"
            + " PRIMARY KEY (id),"
            + " FOREIGN KEY (userId) REFERENCES QueryRunnerTestUser (id) ON DELETE CASCADE)";
    conn.runUpdate(createCardTable, new ConnectionInterface.QueryParams());

    // Run update.
    UserHasManyCreditCards.User user = new UserHasManyCreditCards.User();
    user.id.assign(1);
    user.userName.assign("user0");
    int nRows =
        new SqlRunner()
            .withConnectionReservoir(new BasicConnectionReservoir(factory))
            .withEntity(UserHasManyCreditCards.User.class)
            .runUpdate(user, /*tableName=*/ "QueryRunnerTestUser");
    Assertions.assertEquals(1, nRows);

    UserHasManyCreditCards.Cards card0 = new UserHasManyCreditCards.Cards();
    card0.id.assign(10);
    card0.number.assign("1234");
    card0.userId.assign(1);
    UserHasManyCreditCards.Cards card1 = new UserHasManyCreditCards.Cards();
    card1.id.assign(11);
    card1.number.assign("2234");
    card1.userId.assign(1);

    SqlRunner cardRunner =
        new SqlRunner()
            .withConnectionReservoir(new BasicConnectionReservoir(factory))
            .withEntity(UserHasManyCreditCards.Cards.class);

    nRows = cardRunner.runUpdate(card0, /*tableName=*/ "QueryRunnerTestCard");
    Assertions.assertEquals(1, nRows);

    nRows = cardRunner.runUpdate(card1, /*tableName=*/ "QueryRunnerTestCard");
    Assertions.assertEquals(1, nRows);

    // Run query.
    List<UserHasManyCreditCards> results =
        new SqlRunner()
            .withConnectionReservoir(new BasicConnectionReservoir(factory))
            .withEntity(UserHasManyCreditCards.class)
            .runQuery(
                new SqlQueryBuilder()
                    .queryPiece(
                        "QueryRunnerTestUser userInfo "
                            + "JOIN "
                            + "QueryRunnerTestCard cards "
                            + "ON cards.userId = userInfo.id "
                            + "ORDER BY cards.id DESC"));

    Assertions.assertEquals(1, results.size());
    UserHasManyCreditCards record = results.get(0);
    Assertions.assertEquals((Integer) 1, record.userInfo.id.value());
    Assertions.assertEquals("user0", record.userInfo.userName.value());

    Assertions.assertEquals(2, record.cards.length);
    Assertions.assertEquals((Integer) 11, record.cards[0].id.value());
    Assertions.assertEquals("2234", record.cards[0].number.value());
    Assertions.assertEquals((Integer) 10, record.cards[1].id.value());
    Assertions.assertEquals("1234", record.cards[1].number.value());

    // Clean up.
    conn.runUpdate(dropCardTable, new ConnectionInterface.QueryParams());
    conn.runUpdate(dropUserTable, new ConnectionInterface.QueryParams());
    conn.close();
  }
}
