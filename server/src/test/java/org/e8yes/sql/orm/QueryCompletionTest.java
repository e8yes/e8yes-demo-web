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
package org.e8yes.sql.orm;

import org.e8yes.sql.primitive.SqlInt;
import org.e8yes.sql.primitive.SqlStr;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

/** @author zjz20 */
public class QueryCompletionTest {

  public QueryCompletionTest() {}

  public static class UserHasOneCreditCard {
    public static class UserInfo {
      public SqlInt id = new SqlInt();
      public SqlStr username = new SqlStr();
    }

    public static class Cards {
      public SqlInt id = new SqlInt();
      public SqlStr number = new SqlStr();
    }

    public UserInfo user;
    public Cards card;
  }

  @Test
  public void testOneToOneCase() {
    String query =
        "AUser user JOIN CreditCard card ON "
            + "card.userId = user.id WHERE user.joinDate > '2020-1-1'";
    String completedquery = QueryCompletion.completeSelectQuery(query, UserHasOneCreditCard.class);
    String real =
        "SELECT user.id,user.username,card.id,"
            + "card.number FROM AUser user JOIN CreditCard card ON "
            + "card.userId = user.id WHERE user.joinDate > '2020-1-1'";
    Assertions.assertEquals(real, completedquery);
  }

  public static class UserHasManyCreditCards {
    public static class UserInfo {
      public SqlInt id = new SqlInt();
      public SqlStr username = new SqlStr();
    }

    public static class Cards {
      public SqlInt id = new SqlInt();
      public SqlStr number = new SqlStr();
    }

    public UserInfo user;
    public Cards[] cards;
  }

  @Test
  public void testOneToManyCase() {
    String query =
        "AUser user JOIN CreditCard cards ON "
            + "cards.userId = user.id WHERE user.joinDate > '2020-1-1'";
    String completedquery =
        QueryCompletion.completeSelectQuery(query, UserHasManyCreditCards.class);
    String real =
        "SELECT user.id,user.username,cards.id,"
            + "cards.number FROM AUser user JOIN CreditCard cards ON "
            + "cards.userId = user.id WHERE user.joinDate > '2020-1-1'";
    Assertions.assertEquals(real, completedquery);
  }

  public static class UserEntity {
    public SqlInt id = new SqlInt();
    public SqlStr user_name = new SqlStr();
  }

  @Test
  public void testInsertion() {
    String completedQuery =
        QueryCompletion.completeInsertQuery("auser", UserEntity.class, /*withUpsert=*/ false);
    String expected = "INSERT INTO auser(id,user_name)VALUES(?,?)ON CONFLICT DO NOTHING";
    Assertions.assertEquals(expected, completedQuery);
  }

  @Test
  public void testUpsertion() {
    String completedQuery =
        QueryCompletion.completeInsertQuery("auser", UserEntity.class, /*withUpsert=*/ true);
    String expected =
        "INSERT INTO auser(id,user_name)VALUES(?,?)ON CONFLICT DO UPDATE SET id=?,user_name=?";
    Assertions.assertEquals(expected, completedQuery);
  }
}
