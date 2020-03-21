/**
 * e8yes demo web server.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */
package org.e8yes.service.identity;

/** Handle access control and establish user identity. */
public class AccessControl {
  //  public static Optional<String> genTokenFromCredentialPair(String userName, String passcode)
  //      throws AccessDeniedException {
  //    SqlSession sess = DatabaseConnection.openSession();
  //    AUserMapper userMapper = sess.getMapper(AUserMapper.class);
  //    EtUser user = userMapper.loadByIdOrUserName(null, userName);
  //    if (!BCrypt.checkpw(passcode, user.getPasscode())) {
  //      DatabaseConnection.closeSession(sess);
  //      throw new AccessDeniedException();
  //    }
  //    EtUserGroup userGroup = AUserGroupMapperEx.loadById(sess, user.getGroupId());
  //    DatabaseConnection.closeSession(sess);
  //    try {
  //      return Optional.of(
  //          // TODO: need a secret key provider.
  //          new IdentityContext(user.getId(), userGroup).sign("abc"));
  //    } catch (IllegalArgumentException | UnsupportedEncodingException ex) {
  //      Logger.getLogger(AccessControl.class.getName()).log(Level.SEVERE, null, ex);
  //    }
  //    return Optional.empty();
  //  }
}
