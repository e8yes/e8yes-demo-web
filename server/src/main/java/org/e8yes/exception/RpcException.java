package org.e8yes.exception;

/** Frequently used RPC exception. */
public abstract class RpcException extends Exception {

  @Override
  public abstract String getMessage();

  public abstract int getStatusCode();
}
