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
package org.e8yes.service.identity;

import java.lang.reflect.InvocationTargetException;
import java.sql.SQLException;
import java.util.List;
import java.util.Optional;
import org.e8yes.environment.EnvironmentContextInterface;
import org.e8yes.environment.Initializer;
import org.e8yes.service.Pagination;
import org.e8yes.sql.connection.ConnectionReservoirInterface;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class UserRetrievalTest {

  @BeforeEach
  public void setUp() throws Exception {
    Initializer.init(EnvironmentContextInterface.Environment.Test);
  }

  @AfterEach
  public void tearDown() throws Exception {
    Initializer.cleanUp();
  }

  @Test
  public void retrieveUserEntityTest()
      throws SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();
    UserEntity user0 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(123L), dbConn);
    Assertions.assertNotNull(user0);
    UserEntity user1 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(223L), dbConn);
    Assertions.assertNotNull(user1);

    UserEntity retrieved = UserRetrieval.retrieveUserEntity(/*userId=*/ 123L, dbConn);
    Assertions.assertEquals(user0, retrieved);
  }

  @Test
  public void searchUserByIdPrefixTest()
      throws SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    UserEntity user0 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(12300L), dbConn);
    Assertions.assertNotNull(user0);
    UserEntity user1 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(12301L), dbConn);
    Assertions.assertNotNull(user1);
    UserEntity user2 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(12302L), dbConn);
    Assertions.assertNotNull(user2);
    UserEntity user3 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(12303L), dbConn);
    Assertions.assertNotNull(user3);
    UserEntity user4 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(22300L), dbConn);
    Assertions.assertNotNull(user4);

    List<UserEntity> page0 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.of(123L),
            /*aliasPrefix=*/ Optional.empty(),
            Pagination.newBuilder().setPageNumber(0).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertEquals(2, page0.size());
    Assertions.assertTrue(page0.stream().anyMatch(u -> u.id.value() == 12300L));
    Assertions.assertTrue(page0.stream().anyMatch(u -> u.id.value() == 12301L));

    List<UserEntity> page1 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.of(123L),
            /*aliasPrefix=*/ Optional.empty(),
            Pagination.newBuilder().setPageNumber(1).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertEquals(2, page1.size());
    Assertions.assertTrue(page1.stream().anyMatch(u -> u.id.value() == 12302L));
    Assertions.assertTrue(page1.stream().anyMatch(u -> u.id.value() == 12303L));

    List<UserEntity> page2 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.of(123L),
            /*aliasPrefix=*/ Optional.empty(),
            Pagination.newBuilder().setPageNumber(2).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertTrue(page2.isEmpty());
  }

  @Test
  public void searchUserByAliasPrefixTest()
      throws SQLException, IllegalAccessException, NoSuchMethodException, InstantiationException,
          IllegalArgumentException, InvocationTargetException {
    ConnectionReservoirInterface dbConn =
        Initializer.environmentContext().demowebDbConnections().connectionReservoir();

    UserEntity user0 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(1L), dbConn);
    UserProfile.updateProfile(user0, /*alias=*/ Optional.of("John A"), dbConn);

    UserEntity user1 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(2L), dbConn);
    UserProfile.updateProfile(user1, /*alias=*/ Optional.of("John A"), dbConn);

    UserEntity user2 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(3L), dbConn);
    UserProfile.updateProfile(user2, /*alias=*/ Optional.of("John B"), dbConn);

    UserEntity user3 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(4L), dbConn);
    UserProfile.updateProfile(user3, /*alias=*/ Optional.of("John C"), dbConn);

    UserEntity user4 =
        UserCreation.createBaselineUser("PASS".getBytes(), /*userId=*/ Optional.of(5L), dbConn);
    UserProfile.updateProfile(user4, /*alias=*/ Optional.of("Stieve A"), dbConn);

    List<UserEntity> page0 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.empty(),
            /*aliasPrefix=*/ Optional.of("John"),
            Pagination.newBuilder().setPageNumber(0).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertEquals(2, page0.size());
    Assertions.assertTrue(page0.stream().anyMatch(u -> u.id.value() == 1L));
    Assertions.assertTrue(page0.stream().anyMatch(u -> u.id.value() == 2L));

    List<UserEntity> page1 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.empty(),
            /*aliasPrefix=*/ Optional.of("John"),
            Pagination.newBuilder().setPageNumber(1).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertEquals(2, page1.size());
    Assertions.assertTrue(page1.stream().anyMatch(u -> u.id.value() == 3L));
    Assertions.assertTrue(page1.stream().anyMatch(u -> u.id.value() == 4L));

    List<UserEntity> page2 =
        UserRetrieval.searchUserEntity(
            /*userIdPrefix=*/ Optional.empty(),
            /*aliasPrefix=*/ Optional.of("John"),
            Pagination.newBuilder().setPageNumber(2).setResultPerPage(2).build(),
            dbConn);
    Assertions.assertTrue(page2.isEmpty());
  }
}
