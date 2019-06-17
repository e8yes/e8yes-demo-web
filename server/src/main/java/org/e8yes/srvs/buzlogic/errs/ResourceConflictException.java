package org.e8yes.srvs.buzlogic.errs;

/**
 * ResourceConflictException
 *
 * @author davis
 */
public class ResourceConflictException extends Exception {

  @Override
  public String getMessage() {
    return "Resource Conflict";
  }
}
