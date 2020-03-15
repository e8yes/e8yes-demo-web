package org.e8yes.exception;

/**
 * ResourceMissingException.
 *
 * @author davis
 */
public class ResourceMissingException extends RpcException {

  @Override
  public String getMessage() {
    return "Resource Missing";
  }

  @Override
  public int getStatusCode() {
    return 404;
  }
}
