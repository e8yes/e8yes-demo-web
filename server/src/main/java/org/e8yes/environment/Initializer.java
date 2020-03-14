package org.e8yes.environment;

import org.e8yes.connection.DatabaseConnection;
import org.e8yes.service.identity.User;
import org.e8yes.service.identity.UserGroup;

/** Initialize the environment. */
public class Initializer {

  public static void init(EnvironmentContext ctx) {
    DatabaseConnection.init(ctx);
    UserGroup.init();
    User.init(UserGroup.getContext());
  }
}
