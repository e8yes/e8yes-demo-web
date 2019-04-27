package org.e8yes.srvs.buzlogic;

import java.util.Date;
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

        public static EtUser
                createUser(String userName,
                           String alias,
                           String passWord) {
                AUserMapper mapper = DatabaseConnection.getAUserMapper();

                EtUser user = EtUser
                        .newBuilder()
                        .setId(mapper.reservePk())
                        .setUserName(userName)
                        .setAlias(alias)
                        .setPasscode(BCrypt.hashpw(passWord, BCrypt.gensalt()))
                        .setCreatedAtSec(UtilTime.curTimestampSec())
                        .setStatus(EtUser.UserStatus.ACTIVE)
                        .build();

                mapper.save(user);
                return user;
        }

        public static boolean
                userNameExists(String userName) {
                AUserMapper mapper = DatabaseConnection.getAUserMapper();
                return mapper.loadByIdOrUserName(null, userName) != null;
        }
}
