package org.e8yes.service.identity.dao.mappers;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.service.EtUserGroup;

/**
 * AUserGroupMapperEx workaround for incompatibility between protobuf and mybatis mapper.
 *
 * @author davis
 */
public class AUserGroupMapperEx {

  public static EtUserGroup loadById(SqlSession sess, int id) {
    try {
      ResultSet rs =
          sess.getConnection()
              .createStatement()
              .executeQuery(
                  "SELECT "
                      + "     g.id, "
                      + "     g.description, "
                      + "     g.permissions "
                      + "FROM "
                      + "     auser_group g "
                      + "WHERE "
                      + "     g.id = "
                      + id);
      if (rs.next()) {
        Long[] permVals = (Long[]) rs.getArray("permissions").getArray();
        EtUserGroup.Builder builder =
            EtUserGroup.newBuilder()
                .setId(rs.getInt("id"))
                .setDescription(rs.getString("description"));
        for (long permVal : permVals) {
          builder.addPermissionsValue((int) permVal);
        }
        return builder.build();
      }
    } catch (SQLException ex) {
      Logger.getLogger(AUserGroupMapper.class.getName()).log(Level.SEVERE, null, ex);
    }
    return null;
  }
}
