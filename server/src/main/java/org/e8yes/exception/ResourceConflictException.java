package org.e8yes.exception;

/**
 * ResourceConflictException
 *
 * @author davis
 */
public class ResourceConflictException extends RpcException {

  @Override
  public String getMessage() {
    return "Resource Conflict";
  }

  @Override
  public int getStatusCode() {
    return 409;
  }
}
