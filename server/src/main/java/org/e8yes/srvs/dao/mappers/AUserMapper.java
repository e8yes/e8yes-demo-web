package org.e8yes.srvs.dao.mappers;

import org.apache.ibatis.annotations.Insert;
import org.e8yes.srvs.EtUser;

/**
 *
 * @author daviswen
 */
public interface AUserMapper {

        @Insert("INSERT INTO "
                + "     auser(user_name, alias, passcode, avatar_path) "
                + "VALUES "
                + "     (#{user_name}, #{alias}, #{passcode}, #{avatar_path})")
        public Integer save(EtUser user);
}
