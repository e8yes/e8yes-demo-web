package org.e8yes.exception;

/**
 * HttpException
 *
 * @author davis
 */
public abstract class HttpException extends Exception {

  @Override
  public abstract String getMessage();

  public abstract int getStatusCode();
}
