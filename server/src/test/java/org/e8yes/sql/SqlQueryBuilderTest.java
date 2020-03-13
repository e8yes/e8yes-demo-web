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

import java.util.Date;
import org.e8yes.sql.connection.ConnectionInterface;
import org.e8yes.sql.primitive.SqlDate;
import org.e8yes.sql.primitive.SqlInt;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

public class SqlQueryBuilderTest {

  @Test
  public void testQueryBuilder() {
    SqlQueryBuilder.Placeholder<SqlInt> userId = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder.Placeholder<SqlDate> beginDate = new SqlQueryBuilder.Placeholder();
    SqlQueryBuilder.Placeholder<SqlDate> endDate = new SqlQueryBuilder.Placeholder();

    SqlQueryBuilder builder = new SqlQueryBuilder();
    builder
        .queryPiece("Security s JOIN Image im ON s.id=im.securityId AND ")
        .queryPiece("s.userId=")
        .placeholder(userId)
        .queryPiece(" WHERE im.date>=")
        .placeholder(beginDate)
        .queryPiece(" AND im.date<")
        .placeholder(endDate)
        .queryPiece(" AND im.blockID!=")
        .placeholder(userId);

    Assertions.assertEquals(
        "Security s JOIN Image im ON s.id=im.securityId AND "
            + "s.userId=? WHERE im.date>=? AND im.date<? AND im.blockID!=?",
        builder.jdbcQuery());

    builder.setPlaceholderValue(userId, new SqlInt(1));
    builder.setPlaceholderValue(beginDate, new SqlDate(new Date(100)));
    builder.setPlaceholderValue(endDate, new SqlDate(new Date(101)));

    ConnectionInterface.QueryParams params = builder.queryParams();
    Assertions.assertEquals(4, params.numPositions());
    Assertions.assertEquals((Integer) 1, params.getParam(1).value());
    Assertions.assertEquals(100L, ((Date) params.getParam(2).value()).getTime());
    Assertions.assertEquals(101L, ((Date) params.getParam(3).value()).getTime());
    Assertions.assertEquals((Integer) 1, params.getParam(4).value());

    builder.setPlaceholderValue(userId, new SqlInt(2));

    params = builder.queryParams();
    Assertions.assertEquals(4, params.numPositions());
    Assertions.assertEquals((Integer) 2, params.getParam(1).value());
    Assertions.assertEquals(100L, ((Date) params.getParam(2).value()).getTime());
    Assertions.assertEquals(101L, ((Date) params.getParam(3).value()).getTime());
    Assertions.assertEquals((Integer) 2, params.getParam(4).value());
  }
}
