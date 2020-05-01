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
package org.e8yes.service.socialnetwork;

import java.sql.SQLException;
import java.util.Date;
import org.e8yes.constant.DbTableConstants;
import org.e8yes.exception.ResourceConflictException;
import org.e8yes.service.InvitationStatus;
import org.e8yes.sql.SqlRunner;
import org.e8yes.sql.connection.ConnectionReservoirInterface;

/** Manages the life cycle and the states of friend invitations. */
public class FriendInvitation {

  /**
   * Create a new invitation with status in pending state.
   *
   * @param inviter_user_id User ID of the inviter.
   * @param invitee_user_id User ID of the invitee.
   * @param dbConn Connection to the database server.
   * @return The newly created invitation.
   * @throws SQLException
   * @throws IllegalAccessException
   * @throws ResourceConflictException
   */
  public FriendInvitationEntity createInvitation(
      long inviter_user_id, long invitee_user_id, ConnectionReservoirInterface dbConn)
      throws SQLException, IllegalAccessException, ResourceConflictException {
    FriendInvitationEntity invitation = new FriendInvitationEntity();
    invitation.inviter_user_id.assign(inviter_user_id);
    invitation.invitee_user_id.assign(invitee_user_id);
    invitation.status.assign(InvitationStatus.PENDING_VALUE);
    invitation.created_at.assign(new Date());

    int numRows =
        SqlRunner.runUpdate(
            dbConn,
            DbTableConstants.friendInvitationTable(),
            FriendInvitationEntity.class,
            /*override=*/ false,
            dbConn);
    if (numRows != 1) {
      throw new ResourceConflictException(
          "Invitation where inviter="
              + inviter_user_id
              + ", and invitee="
              + invitee_user_id
              + " doesn't exist.");
    }

    return invitation;
  }
}
