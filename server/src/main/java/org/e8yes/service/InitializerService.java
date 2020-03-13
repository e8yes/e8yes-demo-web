package org.e8yes.service;

import org.e8yes.connection.DatabaseConnection;
import org.e8yes.environment.EnvironmentContext;
import org.e8yes.service.identity.InitializerLogic;

/**
 * Initialize all services.
 *
 * @author davis
 */
public class InitializerService {

  private static EnvironmentContext env_ctx;

  public static void init(EnvironmentContext ctx) {
    env_ctx = ctx;

    DatabaseConnection.init(env_ctx);
    InitializerLogic.init();
  }
}
