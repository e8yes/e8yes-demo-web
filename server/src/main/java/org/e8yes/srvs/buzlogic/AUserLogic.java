package org.e8yes.srvs.buzlogic;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.EtUserGroup;
import org.e8yes.srvs.buzlogic.ctx.AUserGroupContext;
import org.e8yes.srvs.buzlogic.errs.ResourceConflictException;
import org.e8yes.srvs.dao.DatabaseConnection;
import org.e8yes.srvs.dao.mappers.AUserMapper;
import org.mindrot.jbcrypt.BCrypt;

/**
 * Business logic for user management.
 *
 * @author davis
 */
public class AUserLogic {

        public static void
                init(AUserGroupContext userGroupCtx) {
                try {
                        // Create root user.
                        createUser("root",
                                   "superuser",
                                   "c6cefcab-67fc-414d-b105-7ce7f0faae9f",
                                   userGroupCtx.getSystemUserGroup(
                                           AUserGroupContext.SystemUserGroup.SUPER_USER_GROUP));
                } catch (ResourceConflictException ex) {
                        Logger.getLogger(AUserLogic.class.getName()).log(Level.INFO, null, "root user exists.");
                        Logger.getLogger(AUserLogic.class.getName()).log(Level.INFO, null, ex);
                }
        }

        private static EtUser
                createUser(String userName,
                           String alias,
                           String passWord,
                           EtUserGroup userGroup) throws ResourceConflictException {
                SqlSession sess = DatabaseConnection.openSession();
                AUserMapper mapper = sess.getMapper(AUserMapper.class);

                EtUser user = EtUser
                        .newBuilder()
                        .setId(mapper.reservePk())
                        .setUserName(userName)
                        .setAlias(alias)
                        .setPasscode(BCrypt.hashpw(passWord, BCrypt.gensalt()))
                        .setCreatedAtSec(UtilTime.curTimestampSec())
                        .setStatus(EtUser.UserStatus.ACTIVE)
                        .setGroupId(userGroup.getId())
                        .build();

                int rowsAffected = mapper.save(user);
                if (rowsAffected != 1) {
                        throw new ResourceConflictException();
                }
                sess.commit();
                DatabaseConnection.closeSession(sess);
                return user;
        }

        public static EtUser
                createBaselineUser(String userName,
                                   String alias,
                                   String passWord,
                                   AUserGroupContext userGroupCtx) throws ResourceConflictException {
                return createUser(userName,
                                  alias,
                                  passWord,
                                  userGroupCtx.getSystemUserGroup(
                                          AUserGroupContext.SystemUserGroup.BASELINE_USER_GROUP));
        }

        public static boolean
                userNameExists(String userName) {
                SqlSession sess = DatabaseConnection.openSession();
                AUserMapper mapper = sess.getMapper(AUserMapper.class);
                boolean existence = mapper.existsByIdOrUserName(null, userName);
                DatabaseConnection.closeSession(sess);
                return existence;
        }
}
