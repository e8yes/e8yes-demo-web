package org.e8yes.srvs.buzlogic.ctx;

import java.util.Map;
import org.e8yes.srvs.EtUserGroup;

/**
 * Context of a set of known user groups.
 *
 * @author davis
 */
public class AUserGroupContext {

  public enum SystemUserGroup {
    SUPER_USER_GROUP,
    BASELINE_USER_GROUP
  }

  private final Map<SystemUserGroup, EtUserGroup> userGroups;

  public AUserGroupContext(Map<SystemUserGroup, EtUserGroup> userGroups) {
    this.userGroups = userGroups;
  }

  public EtUserGroup getSystemUserGroup(SystemUserGroup group) {
    return userGroups.get(group);
  }
}
