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

#include <algorithm>
#include <cassert>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "demoweb_service/demoweb/common_entity/contact_relation_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/module_socialnetwork/retrieve_contact.h"
#include "demoweb_service/demoweb/proto_cc/user_relation.pb.h"
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
        relation.set_relation(static_cast<UserRelation>(entity.relation.value().value()));
        assert(entity.created_at.value().has_value());
        relation.set_created_at(entity.created_at.value().value());

        UserId target_user_id = entity.dst_user_id.value().value();
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
    SqlQueryBuilder::Placeholder<SqlLong> source_user_id_ph;
    SqlQueryBuilder::Placeholder<SqlLongArr> target_user_ids_ph;
    SqlLong source_user_id_ph_value(source_user_id);
    SqlLongArr target_user_ids_ph_value(target_user_ids);

    // Check contact relation.
    SqlQueryBuilder query;
    query.query_piece(TableNames::ContactRelation())
        .query_piece(" cr")
        .query_piece(" WHERE cr.src_user_id=")
        .placeholder(&source_user_id_ph)
        .query_piece(" AND cr.dst_user_id=ANY(")
        .placeholder(&target_user_ids_ph)
        .query_piece(")");

    query.set_value_to_placeholder(source_user_id_ph, &source_user_id_ph_value);
    query.set_value_to_placeholder(target_user_ids_ph, &target_user_ids_ph_value);

    std::vector<std::tuple<ContactRelationEntity>> result_set =
        Query<ContactRelationEntity>(query, {"cr"}, conns);

    return ParseAllRelations(result_set);
}

std::vector<UserEntity> GetRelatedUsers(UserId source_user_id,
                                        std::vector<UserRelation> const &relations,
                                        std::optional<Pagination> const &pagination,
                                        ConnectionReservoirInterface *conns) {
    SqlQueryBuilder::Placeholder<SqlLong> source_user_id_ph;
    SqlQueryBuilder::Placeholder<SqlIntArr> relations_ph;
    SqlQueryBuilder::Placeholder<SqlInt> limit_ph;
    SqlQueryBuilder::Placeholder<SqlInt> offset_ph;

    SqlLong source_user_id_ph_value(source_user_id);
    SqlIntArr relations_ph_value(/*field_name=*/"");
    *relations_ph_value.value_ptr() = {relations.begin(), relations.end()};
    SqlInt limit_ph_value(/*field_name=*/"");
    SqlInt offset_ph_value(/*field_name=*/"");

    SqlQueryBuilder query;
    query.query_piece(TableNames::AUser())
        .query_piece(" u JOIN ")
        .query_piece(TableNames::ContactRelation())
        .query_piece(" cr ON u.id = cr.dst_user_id ")
        .query_piece(" WHERE cr.src_user_id=")
        .placeholder(&source_user_id_ph)
        .query_piece(" AND cr.relation=ANY(")
        .placeholder(&relations_ph)
        .query_piece(") ORDER BY cr.created_at DESC");

    query.set_value_to_placeholder(source_user_id_ph, &source_user_id_ph_value);
    query.set_value_to_placeholder(relations_ph, &relations_ph_value);

    if (pagination.has_value()) {
        query.query_piece(" LIMIT ").placeholder(&limit_ph);
        query.query_piece(" OFFSET ").placeholder(&offset_ph);

        *limit_ph_value.value_ptr() = pagination->result_per_page();
        *offset_ph_value.value_ptr() = pagination->page_number() * pagination->result_per_page();

        query.set_value_to_placeholder(limit_ph, &limit_ph_value);
        query.set_value_to_placeholder(offset_ph, &offset_ph_value);
    }

    std::vector<std::tuple<UserEntity>> result_set = Query<UserEntity>(query, {"u"}, conns);

    std::vector<UserEntity> users(result_set.size());
    for (unsigned i = 0; i < result_set.size(); i++) {
        users[i] = std::get<0>(result_set[i]);
    }
    return users;
}

} // namespace e8
