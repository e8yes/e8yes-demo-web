package org.e8yes.srvs.buzlogic;

import org.apache.ibatis.session.SqlSession;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.dao.DatabaseConnection;
import org.e8yes.srvs.dao.mappers.AUserMapper;
import org.mindrot.jbcrypt.BCrypt;

/**
 *
 * @author davis
 */
public class AUserLogic {
        
        public static EtUser createUser(String userName, 
                                        String alias,
                                        String passWord) {
                EtUser user = EtUser
                        .newBuilder()
                        .setUserName(userName)
                        .setAlias(alias)
                        .setPasscode(BCrypt.hashpw(passWord, BCrypt.gensalt()))
                        .setStatus(EtUser.UserStatus.ACTIVE)
                        .build();
                SqlSession sess = DatabaseConnection.getSession();
                AUserMapper mapper = sess.getMapper(AUserMapper.class);
                Integer pk = mapper.save(user);
                return user.toBuilder().setId(pk).build();
        }
}
