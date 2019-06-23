package org.e8yes.srvs.buzlogic.errs;

/**
 * ResourceMissingException.
 * 
 * @author davis
 */
public class ResourceMissingException extends HttpException {
        
        @Override
        public String getMessage() {
                return "Resource Missing";
        }

        @Override
        public int getStatusCode() {
                return 404;
        }
}
