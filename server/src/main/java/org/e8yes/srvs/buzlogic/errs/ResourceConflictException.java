package org.e8yes.srvs.buzlogic.errs;

/**
 * ResourceConflictException
 *
 * @author davis
 */
public class ResourceConflictException extends HttpException {

  @Override
  public String getMessage() {
    return "Resource Conflict";
  }

  @Override
  public int getStatusCode() {
    return 409;
  }
}
