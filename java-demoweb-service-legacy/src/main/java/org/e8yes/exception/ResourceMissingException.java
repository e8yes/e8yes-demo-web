package org.e8yes.exception;

/** ResourceMissingException. */
public class ResourceMissingException extends RpcException {

  private final String message;

  public ResourceMissingException() {
    message = "Resource Missing";
  }

  public ResourceMissingException(String message) {
    this.message = message;
  }

  @Override
  public String getMessage() {
    return message;
  }

  @Override
  public int getStatusCode() {
    return 404;
  }
}
