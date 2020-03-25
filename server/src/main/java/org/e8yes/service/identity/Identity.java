package org.e8yes.service.identity;

import java.util.Arrays;

/** Defines the identity context of a user -- Who is that and what can it do? */
public class Identity {

  public long userId;
  public String[] groupNames;

  public Identity(long userId, String[] groupNames) {
    this.userId = userId;
    this.groupNames = groupNames;
  }

  @Override
  public int hashCode() {
    return Long.hashCode(userId);
  }

  @Override
  public boolean equals(Object obj) {
    if (this == obj) {
      return true;
    }
    if (obj == null) {
      return false;
    }
    if (getClass() != obj.getClass()) {
      return false;
    }
    final Identity other = (Identity) obj;
    return this.userId == other.userId;
  }

  @Override
  public String toString() {
    return "Identity{" + "userId=" + userId + ", groupNames=" + Arrays.toString(groupNames) + '}';
  }
}
