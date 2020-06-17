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

#ifndef CONTACT_INVITATION_H
#define CONTACT_INVITATION_H

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief SendInvitation Update the invitation request and recipient list and send out an invitation
 * message. TODO: need to send out a message to the recipient.
 * @param send_message_anyway Send message even if a message has already been sent before if this
 * parameter is set to true.
 * @return true if it an invitation has not been sent before or it hasn't been accepted, otherwise,
 * false.
 */
bool SendInvitation(UserId src_user_id, UserId dst_user_id, bool send_message_anyway,
                    ConnectionReservoirInterface *conns);

} // namespace e8

#endif // CONTACT_INVITATION_H
