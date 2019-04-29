package org.e8yes.srvs;

import org.e8yes.srvs.buzlogic.InitializerLogic;
import org.e8yes.srvs.dao.DatabaseConnection;

/**
 * Initialize all services.
 *
 * @author davis
 */
public class InitializerService {

        private static EnvironmentContext env_ctx;
        
        public static void
                init(EnvironmentContext ctx) {
                env_ctx = ctx;
                
                DatabaseConnection.init(env_ctx);
                InitializerLogic.init();
        }
}
