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

#include <optional>
#include <string>
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/constant/demoweb_database.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/contact_invitation.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "message_queue/publisher/publisher.h"
#include "postgres/query_runner/sql_query_builder.h"
#include "postgres/query_runner/sql_runner.h"
#include "proto_cc/user_relation.pb.h"

class MockMessagePublisher : public e8::MessagePublisherInterface {
  public:
    MockMessagePublisher() = default;
    ~MockMessagePublisher() = default;

    bool Publish(e8::MessageKey /*key*/, e8::RealTimeMessage const &message) {
        published_messages_.push_back(message);
        return true;
    }

    std::vector<e8::RealTimeMessage> published_messages_;
};

bool SendInvitationStorageTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> user1 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/1,
                       env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/2,
                       env.CurrentHostId(), env.DemowebDatabase());

    MockMessagePublisher publisher;
    bool result = e8::SendInvitation(*user1->id.Value(), *user2->id.Value(),
                                     /*send_message_anyway=*/false, env.CurrentHostId(),
                                     std::vector<e8::MessagePublisherInterface *>{&publisher},
                                     env.KeyGen(), env.DemowebDatabase());
    TEST_CONDITION(result);
    TEST_CONDITION(publisher.published_messages_.size() == 1);
    TEST_CONDITION(publisher.published_messages_[0].target_user_id() == *user2->id.Value());
    TEST_CONDITION(publisher.published_messages_[0].real_time_message_id() != 0);
    TEST_CONDITION(publisher.published_messages_[0].created_at() > 0);
    TEST_CONDITION(publisher.published_messages_[0].content().has_invitation_received());

    e8::SqlQueryBuilder forward_query;
    forward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=1 AND cr.dst_user_id=2 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_INVITATION_SENT));
    result = e8::Exists(forward_query, env.DemowebDatabase());
    TEST_CONDITION(result);

    e8::SqlQueryBuilder backward_query;
    backward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=2 AND cr.dst_user_id=1 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_INVITATION_RECEIVED));
    result = e8::Exists(backward_query, env.DemowebDatabase());
    TEST_CONDITION(result);

    return true;
}

bool ProcessInvitationAcceptTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> user1 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/1,
                       env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 =
        e8::CreateUser(/*security_key=*/"key", std::vector<std::string>(), /*user_id=*/2,
                       env.CurrentHostId(), env.DemowebDatabase());

    e8::SendInvitation(*user1->id.Value(), *user2->id.Value(),
                       /*send_message_anyway=*/false, env.CurrentHostId(),
                       std::vector<e8::MessagePublisherInterface *>(), env.KeyGen(),
                       env.DemowebDatabase());

    MockMessagePublisher publisher;
    bool result = e8::ProcessInvitation(*user2->id.Value(), *user1->id.Value(),
                                        /*accept=*/true, env.CurrentHostId(),
                                        std::vector<e8::MessagePublisherInterface *>{&publisher},
                                        env.KeyGen(), env.DemowebDatabase());
    TEST_CONDITION(result);
    TEST_CONDITION(publisher.published_messages_.size() == 1);
    TEST_CONDITION(publisher.published_messages_[0].target_user_id() == *user1->id.Value());
    TEST_CONDITION(publisher.published_messages_[0].real_time_message_id() != 0);
    TEST_CONDITION(publisher.published_messages_[0].created_at() > 0);
    TEST_CONDITION(publisher.published_messages_[0].content().has_invitation_accepted());

    e8::SqlQueryBuilder forward_query;
    forward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=1 AND cr.dst_user_id=2 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_CONTACT));
    result = e8::Exists(forward_query, env.DemowebDatabase());
    TEST_CONDITION(result);

    e8::SqlQueryBuilder backward_query;
    backward_query.QueryPiece(e8::TableNames::ContactRelation())
        .QueryPiece(" cr ")
        .QueryPiece("WHERE cr.src_user_id=2 AND cr.dst_user_id=1 AND cr.relation=" +
                    std::to_string(e8::UserRelation::URL_CONTACT));
    result = e8::Exists(backward_query, env.DemowebDatabase());
    TEST_CONDITION(result);

    return true;
}

int main() {
    e8::BeginTestSuite("contact_invitation");
    e8::RunTest("SendInvitationStorageTest", SendInvitationStorageTest);
    e8::RunTest("ProcessInvitationAcceptTest", ProcessInvitationAcceptTest);
    e8::EndTestSuite();
    return 0;
}
