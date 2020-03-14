package org.e8yes.service.identity;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
import org.apache.ibatis.session.SqlSession;
import org.e8yes.connection.DatabaseConnection;
import org.e8yes.service.EtUserGroup;
import org.e8yes.service.Permission;
import org.e8yes.service.identity.ctx.AUserGroupContext;
import org.e8yes.service.identity.dao.mappers.AUserGroupMapper;

/**
 * Business logic for user group management.
 *
 * @author davis
 */
public class UserGroup {

  private static AUserGroupContext ctx;

  private static Set<Permission> superUserPerms() {
    Set<Permission> perms = new HashSet();
    perms.add(Permission.PERM_USER_GROUP_CREATE);
    perms.add(Permission.PERM_USER_GROUP_REMOVE);
    perms.add(Permission.PERM_USER_GROUP_VIEW);
    perms.add(Permission.PERM_USER_GROUP_ASSIGN);
    perms.add(Permission.PERM_USER_GROUP_UNASSIGN);
    perms.add(Permission.PERM_USER_SELF_CREATE);
    perms.add(Permission.PERM_USER_SELF_REMOVE);
    perms.add(Permission.PERM_USER_CREATE);
    perms.add(Permission.PERM_USER_REMOVE);
    return perms;
  }

  private static Set<Permission> baselineUserPerms() {
    Set<Permission> perms = new HashSet();
    perms.add(Permission.PERM_USER_GROUP_VIEW);
    perms.add(Permission.PERM_USER_SELF_CREATE);
    perms.add(Permission.PERM_USER_SELF_REMOVE);
    return perms;
  }

  public static void init() {
    EtUserGroup superUserGroup = createUserGroup("super_user_group", superUserPerms());
    EtUserGroup baselineUserGroup = createUserGroup("baseline_user_group", baselineUserPerms());
    ctx =
        new AUserGroupContext(
            new HashMap<AUserGroupContext.SystemUserGroup, EtUserGroup>() {
              {
                put(AUserGroupContext.SystemUserGroup.SUPER_USER_GROUP, superUserGroup);
                put(AUserGroupContext.SystemUserGroup.BASELINE_USER_GROUP, baselineUserGroup);
              }
            });
  }

  public static AUserGroupContext getContext() {
    return ctx;
  }

  public static EtUserGroup createUserGroup(String desc, Set<Permission> perms) {
    SqlSession sess = DatabaseConnection.openSession();
    AUserGroupMapper mapper = sess.getMapper(AUserGroupMapper.class);
    int pk = mapper.reservePk();
    EtUserGroup userGroup =
        EtUserGroup.newBuilder().setId(pk).setDescription(desc).addAllPermissions(perms).build();
    mapper.save(userGroup);
    sess.commit();
    DatabaseConnection.closeSession(sess);
    return userGroup;
  }
}
