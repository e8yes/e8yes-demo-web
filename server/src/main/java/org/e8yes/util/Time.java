package org.e8yes.util;

import java.util.Date;

/** @author davis */
public class Time {

  public static long curTimestampSec() {
    return new Date().getTime() / 1000;
  }
}
