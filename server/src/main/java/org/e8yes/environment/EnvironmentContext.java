package org.e8yes.environment;

/**
 * Describe the configuration of environment the program is running on.
 *
 * @author davis
 */
public class EnvironmentContext {

  public enum Mode {
    Prod,
    Test
  }

  public Mode mode = Mode.Test;

  public EnvironmentContext(Mode mode) {
    this.mode = mode;
  }
}
