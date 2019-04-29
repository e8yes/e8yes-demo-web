package org.e8yes.srvs.buzlogic;

import java.util.Date;

/**
 *
 * @author davis
 */
public class UtilTime {

        public static long curTimestampSec() {
                return new Date().getTime() / 1000;
        }
}
