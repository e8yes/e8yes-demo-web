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

#include <cstdint>
#include <ctime>

#include "demoweb_service/demoweb/common_entity/contact_relation_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_socialnetwork/contact_invitation.h"
#include "demoweb_service/demoweb/proto_cc/user_relation.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/sql_runner.h"

namespace e8 {

bool SendInvitation(UserId inviter_id, UserId invitee_id, bool /*send_message_anyway*/,
                    ConnectionReservoirInterface *conns) {
    bool status = true;

    time_t timestamp;
    std::time(&timestamp);

    ContactRelationEntity forward_relation;
    *forward_relation.src_user_id.value_ptr() = inviter_id;
    *forward_relation.dst_user_id.value_ptr() = invitee_id;
    *forward_relation.relation.value_ptr() = URL_INVITATION_SENT;
    *forward_relation.created_at.value_ptr() = timestamp;

    int64_t updated_rows =
        Update(forward_relation, TableNames::ContactRelation(), /*replace=*/false, conns);
    if (updated_rows == 0) {
        status = false;
    }

    ContactRelationEntity backward_relation;
    *backward_relation.src_user_id.value_ptr() = invitee_id;
    *backward_relation.dst_user_id.value_ptr() = inviter_id;
    *backward_relation.relation.value_ptr() = URL_INVITATION_RECEIVED;
    *backward_relation.created_at.value_ptr() = timestamp;
    Update(backward_relation, TableNames::ContactRelation(), /*replace=*/false, conns);

    return status;
}

bool ProcessInvitation(UserId invitee_id, UserId inviter_id, bool accept,
                       ConnectionReservoirInterface *conns) {
    SqlQueryBuilder::Placeholder<SqlLong> invitee_id_ph;
    SqlQueryBuilder::Placeholder<SqlLong> inviter_id_ph;
    SqlQueryBuilder::Placeholder<SqlInt> foward_relation_ph;
    SqlQueryBuilder::Placeholder<SqlInt> backward_relation_ph;
    SqlLong invitee_user_id_ph_value(invitee_id);
    SqlLong inviter_user_id_ph_value(inviter_id);
    SqlInt foward_relation_ph_value(UserRelation::URL_INVITATION_SENT);
    SqlInt backward_relation_ph_value(UserRelation::URL_INVITATION_RECEIVED);

    SqlQueryBuilder query;
    query.query_piece("WHERE (src_user_id=")
        .placeholder(&inviter_id_ph)
        .query_piece(" AND dst_user_id=")
        .placeholder(&invitee_id_ph)
        .query_piece("relation=")
        .placeholder(&foward_relation_ph)
        .query_piece(") OR (src_user_id=")
        .placeholder(&invitee_id_ph)
        .query_piece(" AND dst_user_id=")
        .placeholder(&inviter_id_ph)
        .query_piece(" AND relation=")
        .placeholder(&backward_relation_ph);

    query.set_value_to_placeholder(invitee_id_ph, &invitee_user_id_ph_value);
    query.set_value_to_placeholder(inviter_id_ph, &inviter_user_id_ph_value);
    query.set_value_to_placeholder(foward_relation_ph, &foward_relation_ph_value);
    query.set_value_to_placeholder(backward_relation_ph, &backward_relation_ph_value);

    uint64_t num_deleted = Delete(TableNames::ContactRelation(), query, conns);
    if (num_deleted != 2) {
        return false;
    }

    time_t timestamp;
    std::time(&timestamp);

    if (accept) {
        ContactRelationEntity forward_relation;
        *forward_relation.src_user_id.value_ptr() = inviter_id;
        *forward_relation.dst_user_id.value_ptr() = invitee_id;
        *forward_relation.relation.value_ptr() = URL_CONTACT;
        *forward_relation.created_at.value_ptr() = timestamp;

        Update(forward_relation, TableNames::ContactRelation(), /*replace=*/true, conns);

        ContactRelationEntity backward_relation;
        *backward_relation.src_user_id.value_ptr() = invitee_id;
        *backward_relation.dst_user_id.value_ptr() = inviter_id;
        *backward_relation.relation.value_ptr() = URL_CONTACT;
        *backward_relation.created_at.value_ptr() = timestamp;

        Update(backward_relation, TableNames::ContactRelation(), /*replace=*/true, conns);
    } else {
        ContactRelationEntity rejection_relation;
        *rejection_relation.src_user_id.value_ptr() = inviter_id;
        *rejection_relation.dst_user_id.value_ptr() = invitee_id;
        *rejection_relation.relation.value_ptr() = URL_INVITATION_REJECTED;
        *rejection_relation.created_at.value_ptr() = timestamp;

        Update(rejection_relation, TableNames::ContactRelation(), /*replace=*/true, conns);
    }

    return true;
}

} // namespace e8
