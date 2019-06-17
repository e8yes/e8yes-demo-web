package org.e8yes.srvs.buzlogic.errs;

/**
 * ResourceMissingException.
 *
 * @author davis
 */
public class ResourceMissingException extends Exception {

  @Override
  public String getMessage() {
    return "Resource Missing";
  }
}
