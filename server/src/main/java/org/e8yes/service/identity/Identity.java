package org.e8yes.service.identity;

/** Defines the identity context of a user -- Who is that and what can it do? */
public class Identity {

  public long userId;

  public Identity(long userId) {
    this.userId = userId;
  }
}
