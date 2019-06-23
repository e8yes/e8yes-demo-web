package org.e8yes.srvs.buzlogic.errs;

/**
 * HttpException
 *
 * @author davis
 */
public abstract class HttpException extends Exception {

        @Override
        public abstract String getMessage();

        public abstract int getStatusCode();
}
