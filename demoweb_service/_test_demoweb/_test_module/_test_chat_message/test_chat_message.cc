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
#include "demoweb_service/demoweb/common_entity/chat_message_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/chat_message.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/file.pb.h"
#include "proto_cc/pagination.pb.h"

bool SendAndGetChatMessageTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> user = e8::CreateUser(
        /*security_key=*/std::string(), /*user_group_names=*/std::vector<std::string>(),
        /*user_id=*/1L, env.CurrentHostId(), env.DemowebDatabase());
    e8::MessageChannelEntity message_channel =
        e8::CreateMessageChannel(/*channe_name=*/std::string(), /*description=*/std::string(),
                                 /*encrypted=*/false, /*close_group_channel=*/false,
                                 env.CurrentHostId(), env.DemowebDatabase());
    e8::CreateMessageChannelMembership(*message_channel.id.Value(), *user->id.Value(),
                                       /*member_type=*/e8::MCMT_ADMIN, env.DemowebDatabase());
    std::optional<e8::ChatMessageGroupEntity> chat_message_group = e8::CreateChatMessageGroup(
        *user->id.Value(), *message_channel.id.Value(),
        /*group_title=*/std::string(), /*thread_type=*/e8::CMTT_TEMPORAL, env.CurrentHostId(),
        env.MessageChannelPbac(), env.DemowebDatabase());

    // Send out one message and verify the content.
    std::optional<e8::SendChatMessageResult> result = e8::SendChatMessage(
        *user->id.Value(), *chat_message_group->id.Value(), /*texts=*/{"message1"},
        /*media_file_formats=*/std::vector<e8::FileFormat>(),
        /*binary_file_formats=*/std::vector<e8::FileFormat>(), env.MessageChannelPbac(),
        env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(result.has_value());
    TEST_CONDITION(result->message.message_seq_id() != 0);
    TEST_CONDITION(result->message.thread_id() == *chat_message_group->id.Value());
    TEST_CONDITION(result->message.created_at() > 0);
    TEST_CONDITION(result->message.has_sender());
    TEST_CONDITION(result->message.sender().user_id() == *user->id.Value());
    TEST_CONDITION(result->message.texts().size() == 1);
    TEST_CONDITION(result->message.texts()[0] == "message1");

    // Send out two more messages.
    e8::SendChatMessage(*user->id.Value(), *chat_message_group->id.Value(), /*texts=*/{"message2"},
                        /*media_file_formats=*/std::vector<e8::FileFormat>(),
                        /*binary_file_formats=*/std::vector<e8::FileFormat>(),
                        env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());
    e8::SendChatMessage(*user->id.Value(), *chat_message_group->id.Value(), /*texts=*/{"message3"},
                        /*media_file_formats=*/std::vector<e8::FileFormat>(),
                        /*binary_file_formats=*/std::vector<e8::FileFormat>(),
                        env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());

    // Fetch those messages back.
    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);

    std::vector<e8::ChatMessageEntry> page1_messages =
        e8::GetChatMessages(*user->id.Value(), *chat_message_group->id.Value(), page1,
                            env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(page1_messages.size() == 2);

    TEST_CONDITION(page1_messages[0].message_seq_id() != 0);
    TEST_CONDITION(page1_messages[0].thread_id() == *chat_message_group->id.Value());
    TEST_CONDITION(page1_messages[0].created_at() > 0);
    TEST_CONDITION(page1_messages[0].has_sender());
    TEST_CONDITION(page1_messages[0].sender().user_id() == *user->id.Value());
    TEST_CONDITION(page1_messages[0].texts().size() == 1);
    TEST_CONDITION(page1_messages[0].texts()[0] == "message1");

    TEST_CONDITION(page1_messages[1].texts().size() == 1);
    TEST_CONDITION(page1_messages[1].texts()[0] == "message2");

    e8::Pagination page2;
    page2.set_page_number(1);
    page2.set_result_per_page(2);

    std::vector<e8::ChatMessageEntry> page2_messages =
        e8::GetChatMessages(*user->id.Value(), *chat_message_group->id.Value(), page2,
                            env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(page2_messages.size() == 1);

    TEST_CONDITION(page2_messages[0].texts().size() == 1);
    TEST_CONDITION(page2_messages[0].texts()[0] == "message3");

    return true;
}

int main() {
    e8::BeginTestSuite("chat_message");
    e8::RunTest("SendAndGetChatMessageTest", SendAndGetChatMessageTest);
    e8::EndTestSuite();
    return 0;
}
