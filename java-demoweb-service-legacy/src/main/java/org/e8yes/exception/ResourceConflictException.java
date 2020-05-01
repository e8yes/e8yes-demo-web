package org.e8yes.exception;

/** ResourceConflictException */
public class ResourceConflictException extends RpcException {

  private final String message;

  public ResourceConflictException() {
    message = "Resource Conflict";
  }

  public ResourceConflictException(String message) {
    this.message = message;
  }

  @Override
  public String getMessage() {
    return message;
  }

  @Override
  public int getStatusCode() {
    return 409;
  }
}
