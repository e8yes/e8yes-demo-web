package org.e8yes.srvs.buzlogic;

import com.auth0.jwt.JWT;
import java.io.UnsupportedEncodingException;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.EtUserGroup;
import org.e8yes.srvs.buzlogic.ctx.IdentityContext;
import org.e8yes.srvs.buzlogic.errs.AccessDeniedException;
import org.e8yes.srvs.dao.DatabaseConnection;
import org.e8yes.srvs.dao.mappers.AUserGroupMapper;
import org.e8yes.srvs.dao.mappers.AUserMapper;
import org.mindrot.jbcrypt.BCrypt;

/**
 * Business logic to handle access control and establish user identity.
 *
 * @author davis
 */
public class AccessControlLogic {

        public static Optional<String>
                genTokenFromCredentialPair(String userName, String passcode) throws AccessDeniedException {
                SqlSession sess = DatabaseConnection.openSession();
                AUserMapper userMapper = sess.getMapper(AUserMapper.class);
                EtUser user = userMapper.loadByIdOrUserName(null, userName);
                if (!BCrypt.checkpw(passcode, user.getPasscode())) {
                        DatabaseConnection.closeSession(sess);
                        throw new AccessDeniedException();
                }
                AUserGroupMapper userGroupMapper = sess.getMapper(AUserGroupMapper.class);
                EtUserGroup userGroup = userGroupMapper.loadById(user.getGroupId());
                DatabaseConnection.closeSession(sess);
                try {
                        return Optional.of(
                                // TODO: need a secret key provider.
                                new IdentityContext(user.getId(), userGroup)
                                        .sign("abc")
                        );
                } catch (IllegalArgumentException | UnsupportedEncodingException ex) {
                        Logger.getLogger(AccessControlLogic.class.getName()).log(Level.SEVERE, null, ex);
                }
                return Optional.empty();
        }
}
