package org.e8yes.srvs.buzlogic.errs;

/**
 * AccessDeniedException
 *
 * @author davis
 */
public class AccessDeniedException extends HttpException {

  @Override
  public String getMessage() {
    return "Access Denied";
  }

  @Override
  public int getStatusCode() {
    return 401;
  }
}
