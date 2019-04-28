package org.e8yes.srvs.dao.mappers;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Result;
import org.apache.ibatis.annotations.ResultType;
import org.apache.ibatis.annotations.Results;
import org.apache.ibatis.annotations.Select;
import org.e8yes.srvs.EtUser;

/**
 *
 * @author daviswen
 */
public interface AUserMapper {

        @Select("SELECT nextval('auser_id_seq') ")
        public long
                reservePk();

        @Insert("INSERT INTO "
                + "     auser(id, "
                + "           user_name, "
                + "           alias, "
                + "           passcode, "
                + "           created_at, "
                + "           avatar_path) "
                + "VALUES "
                + "     (#{id}, "
                + "      #{userName}, "
                + "      #{alias}, "
                + "      #{passcode}, "
                + "      TO_TIMESTAMP(#{createdAtSec}) AT TIME ZONE 'UTC', "
                + "      #{avatarPath})")
        public int
                save(EtUser user);

        @Select("SELECT "
                + "     u.id, "
                + "     u.user_name, "
                + "     u.alias, "
                + "     u.passcode, "
                + "     CAST(EXTRACT(EPOCH FROM u.created_at) AS BIGINT) AS created_at_sec, "
                + "     u.avatar_path, "
                + "     u.status "
                + "FROM "
                + "     auser u "
                + "WHERE "
                + "     u.id = #{id} OR u.user_name = #{userName} ")
        @Results(value = {
                @Result(property = "id_", column = "id"),
                @Result(property = "userName_", column = "user_name"),
                @Result(property = "alias_", column = "alias"),
                @Result(property = "passcode_", column = "passcode"),
                @Result(property = "createdAtSec_", column = "created_at_sec"),
                @Result(property = "avatarPath_", column = "avatar_path"),
                @Result(property = "status_", column = "status")
        })
        public EtUser
                loadByIdOrUserName(@Param("id") Long id,
                                   @Param("userName") String userName);

        @Select("SELECT "
                + "     COUNT(u.id) > 0 "
                + "FROM "
                + "     auser u "
                + "WHERE "
                + "     u.id = #{id} OR u.user_name = #{userName} ")
        public boolean
                existsByIdOrUserName(@Param("id") Long id,
                                     @Param("userName") String userName);
}
