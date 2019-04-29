package org.e8yes.srvs.buzlogic;

import java.sql.SQLException;
import java.sql.Savepoint;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.EnvironmentContext;
import org.e8yes.srvs.dao.DatabaseConnection;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.buzlogic.ctx.AUserGroupContext;
import org.e8yes.srvs.dao.mappers.AUserMapper;

/**
 *
 * @author davis
 */
public class AUserLogicTest {

        public AUserLogicTest() {
        }

        @BeforeAll
        public static void
                setUpClass() {
        }

        @AfterAll
        public static void
                tearDownClass() {
        }

        @BeforeEach
        public void
                setUp() throws SQLException {
                DatabaseConnection.init(new EnvironmentContext(EnvironmentContext.Mode.Test));
                InitializerLogic.init();
        }

        @AfterEach
        public void
                tearDown() throws SQLException {
                DatabaseConnection.deleteAllData();
        }

        @Test
        public void
                createUserTest() {
                AUserGroupContext ctx = AUserGroupLogic.getContext();
                Assertions.assertNotNull(ctx);

                String user0Name = "USER0";
                String user0Alias = "";
                String user0PassWord = "PASS";
                EtUser user = AUserLogic.createBaselineUser(user0Name,
                                                            user0Alias,
                                                            user0PassWord,
                                                            ctx);
                user.getUserName();
                Assertions.assertNotEquals(0, user.getId());
                Assertions.assertEquals(user0Name, user.getUserName());
                Assertions.assertEquals(user0Alias, user.getAlias());
                Assertions.assertNotNull(user.getPasscode());

                SqlSession sess = DatabaseConnection.openSession();
                AUserMapper mapper = sess.getMapper(AUserMapper.class);
                EtUser loadedUser = mapper.loadByIdOrUserName(user.getId(), null);
                Assertions.assertNotNull(loadedUser);
                Assertions.assertEquals(loadedUser, user);

                Assertions.assertTrue(AUserLogic.userNameExists(user0Name));
        }
}
