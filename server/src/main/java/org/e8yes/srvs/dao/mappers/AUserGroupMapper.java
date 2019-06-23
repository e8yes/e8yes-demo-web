package org.e8yes.srvs.dao.mappers;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Result;
import org.apache.ibatis.annotations.Results;
import org.apache.ibatis.annotations.Select;
import org.e8yes.srvs.EtUser;
import org.e8yes.srvs.EtUserGroup;

/**
 * EtUserGroup database entity mapping.
 *
 * @author davis
 */
public interface AUserGroupMapper {

        @Select("SELECT nextval('auser_group_id_seq') ")
        public int
                reservePk();

        @Insert("INSERT INTO "
                + "     auser_group(id, "
                + "                 description, "
                + "                 permissions) "
                + "VALUES "
                + "     (#{userGroup.id}, "
                + "      #{userGroup.description}, "
                + "      #{userGroup.permissionsValueList, typeHandler=org.e8yes.srvs.dao.mappers.handlers.IntegerListTypeHandler}) "
                + "ON CONFLICT "
                + "     DO NOTHING ")
        public int
                save(@Param("userGroup") EtUserGroup userGroup);
                
        @Select("SELECT "
                + "     g.id, "
                + "     g.description, "
                + "     g.permissions "
                + "FROM "
                + "     ausergroup g "
                + "WHERE "
                + "     g.id = #{id} ")
        @Results(value = {
                @Result(property = "id_", column = "id"),
                @Result(property = "description_", column = "description")
        })
        public EtUserGroup
                loadById(@Param("id") int id);
}
