package org.e8yes.service.identity.dao.mappers;

import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;
import org.e8yes.service.EtUserGroup;

/**
 * EtUserGroup database entity mapping.
 *
 * @author davis
 */
public interface AUserGroupMapper {

  @Select("SELECT nextval('auser_group_id_seq') ")
  public int reservePk();

  @Insert(
      "INSERT INTO "
          + "     auser_group(id, "
          + "                 description, "
          + "                 permissions) "
          + "VALUES "
          + "     (#{userGroup.id}, "
          + "      #{userGroup.description}, "
          + "      #{userGroup.permissionsValueList, typeHandler=org.e8yes.srvs.dao.mappers.handlers.IntegerListTypeHandler}) "
          + "ON CONFLICT "
          + "     DO NOTHING ")
  public int save(@Param("userGroup") EtUserGroup userGroup);
}
