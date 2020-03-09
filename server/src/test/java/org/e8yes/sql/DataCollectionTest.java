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
import org.e8yes.sql.resultset.MockResultSet;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class DataCollectionTest {

  public static class UserHasOneCreditCard {
    public static class UserInfo {
      public Integer id;
      public String username;
    }

    public static class Cards {
      public Integer id;
      public String number;
    }

    public UserInfo user;
    public Cards card;
  }

  @Test
  public void testOneToOneCase()
      throws NoSuchMethodException, InstantiationException, IllegalAccessException,
          IllegalArgumentException, InvocationTargetException, SQLException {
    MockResultSet rs = new MockResultSet(/*numCells=*/ 4);
    rs.addRecord(new Object[] {1, "user_name", 11, "card_number"});

    List<UserHasOneCreditCard> results = DataCollection.collect(rs, UserHasOneCreditCard.class);

    Assertions.assertEquals(1, results.size());
    UserHasOneCreditCard record = results.iterator().next();

    Assertions.assertEquals((Integer) 1, record.user.id);
    Assertions.assertEquals("user_name", record.user.username);
    Assertions.assertEquals((Integer) 11, record.card.id);
    Assertions.assertEquals("card_number", record.card.number);
  }
}
