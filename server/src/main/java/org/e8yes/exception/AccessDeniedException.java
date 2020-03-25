package org.e8yes.exception;

/** AccessDeniedException */
public class AccessDeniedException extends RpcException {

  private final String message;

  public AccessDeniedException() {
    message = "Access Denied";
  }

  public AccessDeniedException(String message) {
    this.message = message;
  }

  @Override
  public String getMessage() {
    return message;
  }

  @Override
  public int getStatusCode() {
    return 401;
  }
}
