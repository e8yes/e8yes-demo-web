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
#include "demoweb_service/demoweb/module/retrieve_contact.h"
#include "proto_cc/user_relation.pb.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "postgres/query_runner/reflection/sql_primitives.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"

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

std::vector<UserEntity> GetRelatedUsers(UserId source_user_id,
                                        std::vector<UserRelation> const &relations,
                                        std::optional<Pagination> const &pagination,
                                        ConnectionReservoirInterface *conns) {
    SqlIntArr relations_ph_value(/*field_name=*/"");
    *relations_ph_value.ValuePtr() = {relations.begin(), relations.end()};
    SqlInt limit_ph_value(/*field_name=*/"");
    SqlInt offset_ph_value(/*field_name=*/"");

    SqlQueryBuilder query;

    SqlQueryBuilder::Placeholder<SqlLong> source_user_id_ph;
    SqlQueryBuilder::Placeholder<SqlIntArr> relations_ph;
    SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
    SqlQueryBuilder::Placeholder<SqlInt> offset_ph;

    query.QueryPiece(TableNames::AUser())
        .QueryPiece(" u JOIN ")
        .QueryPiece(TableNames::ContactRelation())
        .QueryPiece(" cr ON u.id = cr.dst_user_id ")
        .QueryPiece(" WHERE cr.src_user_id=")
        .Holder(&source_user_id_ph)
        .QueryPiece(" AND cr.relation=ANY(")
        .Holder(&relations_ph)
        .QueryPiece(") ORDER BY cr.last_interaction_at DESC");

    query.SetValueToPlaceholder(source_user_id_ph, std::make_shared<SqlLong>(source_user_id));
    query.SetValueToPlaceholder(relations_ph, std::make_shared<SqlIntArr>(std::vector<int>{
                                                  relations.begin(), relations.end()}));

    if (pagination.has_value()) {
        query.QueryPiece(" LIMIT ").Holder(&limit_ph);
        query.QueryPiece(" OFFSET ").Holder(&offset_ph);

        query.SetValueToPlaceholder(limit_ph,
                                    std::make_shared<SqlInt>(pagination->result_per_page()));
        query.SetValueToPlaceholder(
            offset_ph,
            std::make_shared<SqlInt>(pagination->page_number() * pagination->result_per_page()));
    }

    std::vector<std::tuple<UserEntity>> result_set = Query<UserEntity>(query, {"u"}, conns);

    std::vector<UserEntity> users(result_set.size());
    for (unsigned i = 0; i < result_set.size(); i++) {
        users[i] = std::get<0>(result_set[i]);
    }
    return users;
}

} // namespace e8
