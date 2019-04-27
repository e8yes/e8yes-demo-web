package org.e8yes.srvs.buzlogic;

import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.dao.DatabaseConnection;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.dao.mappers.AUserMapper;

/**
 *
 * @author davis
 */
public class AUserLogicTest {

        private SqlSession sess;

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
                setUp() {
                sess = DatabaseConnection.getSession();
        }

        @AfterEach
        public void
                tearDown() {
                sess.rollback();
        }

        @Test
        public void
                createUserTest() {
                String user0Name = "USER0";
                String user0Alias = "";
                String user0PassWord = "PASS";
                EtUser user = AUserLogic.createUser(user0Name,
                                                    user0Alias,
                                                    user0PassWord);
                user.getUserName();
                Assertions.assertNotEquals(0, user.getId());
                Assertions.assertEquals(user0Name, user.getUserName());
                Assertions.assertEquals(user0Alias, user.getAlias());
                Assertions.assertNotNull(user.getPasscode());

                AUserMapper mapper = DatabaseConnection.getAUserMapper();
                EtUser loadedUser = mapper.loadByIdOrUserName(user.getId(), null);
                Assertions.assertNotNull(loadedUser);
                Assertions.assertEquals(loadedUser, user);
                
                Assertions.assertTrue(AUserLogic.userNameExists(user0Name));
        }
}
