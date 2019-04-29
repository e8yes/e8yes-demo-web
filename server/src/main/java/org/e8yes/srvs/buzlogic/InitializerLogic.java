package org.e8yes.srvs.buzlogic;

/**
 * Initialize all business logic.
 *
 * @author davis
 */
public class InitializerLogic {

        public static void
                init() {
                AUserGroupLogic.init();
                AUserLogic.init(AUserGroupLogic.getContext());
        }
}
