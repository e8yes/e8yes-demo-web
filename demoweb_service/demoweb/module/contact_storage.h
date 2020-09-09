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

#ifndef RETRIEVE_CONTACT_H
#define RETRIEVE_CONTACT_H

#include <optional>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "keygen/key_generator_interface.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/pagination.pb.h"
#include "proto_cc/user_relation.pb.h"

namespace e8 {

using UserRelations = std::vector<UserRelationRecord>;

/**
 * @brief GetUsersRelation Get all the directed user relations between the source user and a list of
 * target users. The list of relation is per target user is ordered by creation timestamp in
 * descending order.
 * @return A map containing pairs of (target_user_id, relation_set).
 */
std::unordered_map<UserId, UserRelations> GetUsersRelations(UserId source_user_id,
                                                            std::vector<UserId> target_user_ids,
                                                            ConnectionReservoirInterface *conns);

/**
 * @brief CreateContact Creates a contact between the inviter and invitee.
 *
 * @return true if the contact has not existed.
 */
bool CreateContact(UserId const inviter_id, UserId const invitee_id,
                   ConnectionReservoirInterface *conns);

/**
 * @brief RejectContact Mark that the inviter is being rejected by the invitee.
 *
 * @return true if the rejection has not been marked before.
 */
bool RejectContact(UserId const inviter_id, UserId const invitee_id,
                   ConnectionReservoirInterface *conns);

/**
 * @brief DeleteContact Delete a contact between the viewer and the target user.
 *
 * @return true if there's contact being deleted.
 */
bool DeleteContact(UserId const viewer_id, UserId const target_user_id,
                   ConnectionReservoirInterface *conns);

} // namespace e8

#endif // RETRIEVE_CONTACT_H
