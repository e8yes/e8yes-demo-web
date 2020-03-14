package org.e8yes.service.identity;

import java.io.UnsupportedEncodingException;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.exception.AccessDeniedException;
import org.e8yes.service.EtUser;
import org.e8yes.service.EtUserGroup;
import org.e8yes.service.identity.ctx.IdentityContext;
import org.e8yes.service.identity.dao.mappers.AUserGroupMapperEx;
import org.e8yes.service.identity.dao.mappers.AUserMapper;
import org.mindrot.jbcrypt.BCrypt;

/**
 * Business logic to handle access control and establish user identity.
 *
 * @author davis
 */
public class AccessControl {

  public static Optional<String> genTokenFromCredentialPair(String userName, String passcode)
      throws AccessDeniedException {
    SqlSession sess = DatabaseConnection.openSession();
    AUserMapper userMapper = sess.getMapper(AUserMapper.class);
    EtUser user = userMapper.loadByIdOrUserName(null, userName);
    if (!BCrypt.checkpw(passcode, user.getPasscode())) {
      DatabaseConnection.closeSession(sess);
      throw new AccessDeniedException();
    }
    EtUserGroup userGroup = AUserGroupMapperEx.loadById(sess, user.getGroupId());
    DatabaseConnection.closeSession(sess);
    try {
      return Optional.of(
          // TODO: need a secret key provider.
          new IdentityContext(user.getId(), userGroup).sign("abc"));
    } catch (IllegalArgumentException | UnsupportedEncodingException ex) {
      Logger.getLogger(AccessControl.class.getName()).log(Level.SEVERE, null, ex);
    }
    return Optional.empty();
  }
}
