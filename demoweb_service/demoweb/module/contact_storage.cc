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

#include <algorithm>
#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/contact_relation_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module/contact_storage.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/user_relation.pb.h"

namespace e8 {

namespace {

std::unordered_map<UserId, UserRelations>
ParseAllRelations(std::vector<std::tuple<ContactRelationEntity>> const &result_set) {
    std::unordered_map<UserId, UserRelations> result;

    for (const auto &entry : result_set) {
        ContactRelationEntity const &entity = std::get<0>(entry);

        UserRelationRecord relation;
        relation.set_relation(static_cast<UserRelation>(entity.relation.Value().value()));
        assert(entity.created_at.Value().has_value());
        relation.set_created_at(entity.created_at.Value().value());

        UserId target_user_id = entity.dst_user_id.Value().value();
        auto it = result.find(target_user_id);
        if (it == result.end()) {
            it = result.insert(std::make_pair(target_user_id, UserRelations())).first;
        }
        it->second.push_back(relation);
    }

    for (auto &entry : result) {
        std::sort(entry.second.begin(), entry.second.end(),
                  [](UserRelationRecord const &a, UserRelationRecord const &b) {
                      return a.created_at() > b.created_at();
                  });
    }

    return result;
}

} // namespace

std::unordered_map<UserId, UserRelations> GetUsersRelations(UserId source_user_id,
                                                            std::vector<UserId> target_user_ids,
                                                            ConnectionReservoirInterface *conns) {
    // Check contact relation.
    SqlQueryBuilder query;

    SqlQueryBuilder::Placeholder<SqlLong> source_user_id_ph;
    SqlQueryBuilder::Placeholder<SqlLongArr> target_user_ids_ph;

    query.QueryPiece(TableNames::ContactRelation())
        .QueryPiece(" cr")
        .QueryPiece(" WHERE cr.src_user_id=")
        .Holder(&source_user_id_ph)
        .QueryPiece(" AND cr.dst_user_id=ANY(")
        .Holder(&target_user_ids_ph)
        .QueryPiece(")");

    query.SetValueToPlaceholder(source_user_id_ph, std::make_shared<SqlLong>(source_user_id));
    query.SetValueToPlaceholder(target_user_ids_ph, std::make_shared<SqlLongArr>(target_user_ids));

    std::vector<std::tuple<ContactRelationEntity>> result_set =
        Query<ContactRelationEntity>(query, {"cr"}, conns);

    return ParseAllRelations(result_set);
}

bool CreateContact(UserId const inviter_id, UserId const invitee_id,
                   ConnectionReservoirInterface *conns) {
    std::time_t timestamp;
    std::time(&timestamp);

    ContactRelationEntity forward_relation;
    *forward_relation.src_user_id.ValuePtr() = inviter_id;
    *forward_relation.dst_user_id.ValuePtr() = invitee_id;
    *forward_relation.relation.ValuePtr() = URL_CONTACT;
    *forward_relation.created_at.ValuePtr() = timestamp;
    *forward_relation.last_interaction_at.ValuePtr() = timestamp;

    int64_t num_rows_updated = 0;
    num_rows_updated +=
        Update(forward_relation, TableNames::ContactRelation(), /*replace=*/false, conns);

    ContactRelationEntity backward_relation;
    *backward_relation.src_user_id.ValuePtr() = invitee_id;
    *backward_relation.dst_user_id.ValuePtr() = inviter_id;
    *backward_relation.relation.ValuePtr() = URL_CONTACT;
    *backward_relation.created_at.ValuePtr() = timestamp;
    *backward_relation.last_interaction_at.ValuePtr() = timestamp;

    num_rows_updated +=
        Update(backward_relation, TableNames::ContactRelation(), /*replace=*/false, conns);

    return num_rows_updated == 2;
}

bool RejectContact(UserId const inviter_id, UserId const invitee_id,
                   ConnectionReservoirInterface *conns) {
    std::time_t timestamp;
    std::time(&timestamp);

    ContactRelationEntity rejection_relation;
    *rejection_relation.src_user_id.ValuePtr() = inviter_id;
    *rejection_relation.dst_user_id.ValuePtr() = invitee_id;
    *rejection_relation.relation.ValuePtr() = URL_INVITATION_REJECTED;
    *rejection_relation.created_at.ValuePtr() = timestamp;
    *rejection_relation.last_interaction_at.ValuePtr() = timestamp;

    int64_t num_rows_updated =
        Update(rejection_relation, TableNames::ContactRelation(), /*replace=*/false, conns);

    return num_rows_updated == 1;
}

bool DeleteContact(UserId const viewer_id, UserId const target_user_id,
                   ConnectionReservoirInterface *conns) {
    SqlQueryBuilder query;
    SqlQueryBuilder::Placeholder<SqlLong> user_id1_ph;
    SqlQueryBuilder::Placeholder<SqlLong> user_id2_ph;
    SqlQueryBuilder::Placeholder<SqlInt> relation_ph;
    query.QueryPiece("WHERE ((src_user_id=")
        .Holder(&user_id1_ph)
        .QueryPiece(" AND dst_user_id=")
        .Holder(&user_id2_ph)
        .QueryPiece(") OR (dst_user_id=")
        .Holder(&user_id1_ph)
        .QueryPiece(" AND src_user_id=")
        .Holder(&user_id2_ph)
        .QueryPiece(")) AND relation=")
        .Holder(&relation_ph);

    query.SetValueToPlaceholder(user_id1_ph, std::make_shared<SqlLong>(viewer_id));
    query.SetValueToPlaceholder(user_id2_ph, std::make_shared<SqlLong>(target_user_id));
    query.SetValueToPlaceholder(relation_ph, std::make_shared<SqlInt>(URL_CONTACT));

    int64_t num_rows = Delete(TableNames::ContactRelation(), query, conns);
    return num_rows == 2;
}

} // namespace e8
