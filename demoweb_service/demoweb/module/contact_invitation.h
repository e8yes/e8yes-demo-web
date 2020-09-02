/**
 * e8yes demo web.
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

#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/host_id.h"
#include "keygen/key_generator_interface.h"
#include "message_queue/publisher/publisher.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"

namespace e8 {

/**
 * @brief SendInvitation Update the invitation request and recipient list and send out an invitation
 * message.
 *
 * @param send_message_anyway Send message even if a message has already been sent before if this
 * parameter is set to true.
 * @return true if it an invitation has not been sent before or it hasn't been accepted, otherwise,
 * false.
 */
bool SendInvitation(UserId inviter_id, UserId invitee_id, HostId const host_id,
                    bool send_message_anyway,
                    std::vector<MessagePublisherInterface *> const &publishers,
                    KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns);

/**
 * @brief ProcessInvitation Accepts or rejects an invitation. If accepted, the invitation will be
 * removed and a contact relation will be established between the invitee and inviter. Otherwise,
 * the invitation will be removed and an invitation rejection relation will be established instead.
 * @return false if the invitation can't be found.
 */
bool ProcessInvitation(UserId invitee_id, UserId inviter_id, HostId const host_id, bool accept,
                       std::vector<MessagePublisherInterface *> const &publishers,
                       KeyGeneratorInterface *key_gen, ConnectionReservoirInterface *conns);

} // namespace e8

#endif // CONTACT_INVITATION_H
