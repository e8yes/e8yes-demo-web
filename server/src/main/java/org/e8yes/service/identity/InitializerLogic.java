package org.e8yes.service.identity;

/**
 * Initialize all business logic.
 *
 * @author davis
 */
public class InitializerLogic {

  public static void init() {
    UserGroup.init();
    User.init(UserGroup.getContext());
  }
}
