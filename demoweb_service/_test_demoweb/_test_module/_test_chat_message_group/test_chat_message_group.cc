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

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/chat_message_entity.h"
#include "demoweb_service/demoweb/common_entity/chat_message_group_entity.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/chat_message_group.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/chat_message_storage.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "proto_cc/chat_message.pb.h"
#include "proto_cc/pagination.pb.h"

bool CreateChatMessageGroupTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> creator =
        e8::CreateUser(/*security_key=*/"", /*user_group_names=*/std::vector<std::string>(),
                       /*user_id=*/1L, env.CurrentHostId(), env.DemowebDatabase());
    e8::MessageChannelEntity message_channel = e8::CreateMessageChannel(
        /*channel_name=*/"Test channel name",
        /*description=*/"Test channel description", /*encrypted=*/false,
        /*close_group_channel=*/false, env.CurrentHostId(), env.DemowebDatabase());
    e8::CreateMessageChannelMembership(/*channel_id=*/*message_channel.id.Value(),
                                       *creator->id.Value(), /*member_type=*/e8::MCMT_ADMIN,
                                       env.DemowebDatabase());

    std::optional<e8::ChatMessageGroupEntity> chat_message_group =
        e8::CreateChatMessageGroup(*creator->id.Value(), *message_channel.id.Value(),
                                   /*group_title=*/"Test group title",
                                   /*thread_type=*/e8::CMTT_TEMPORAL, env.CurrentHostId(),
                                   env.MessageChannelPbac(), env.DemowebDatabase());
    TEST_CONDITION(chat_message_group.has_value());

    return true;
}

bool GetChatMessageGroupsWithChatMessageSummaryListTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> creator =
        e8::CreateUser(/*security_key=*/"", /*user_group_names=*/std::vector<std::string>(),
                       /*user_id=*/1L, env.CurrentHostId(), env.DemowebDatabase());
    e8::MessageChannelEntity message_channel = e8::CreateMessageChannel(
        /*channel_name=*/"Test channel name",
        /*description=*/"Test channel description", /*encrypted=*/false,
        /*close_group_channel=*/false, env.CurrentHostId(), env.DemowebDatabase());
    e8::CreateMessageChannelMembership(/*channel_id=*/*message_channel.id.Value(),
                                       *creator->id.Value(), /*member_type=*/e8::MCMT_ADMIN,
                                       env.DemowebDatabase());

    e8::MessageChannelEntity empty_message_channel = e8::CreateMessageChannel(
        /*channel_name=*/"Empty test channel name",
        /*description=*/"Empyt test channel description", /*encrypted=*/false,
        /*close_group_channel=*/false, env.CurrentHostId(), env.DemowebDatabase());
    e8::CreateMessageChannelMembership(/*channel_id=*/*empty_message_channel.id.Value(),
                                       *creator->id.Value(), /*member_type=*/e8::MCMT_ADMIN,
                                       env.DemowebDatabase());

    e8::ChatMessageGroupEntity group1 = e8::CreateChatMessageGroup(
        *creator->id.Value(), *message_channel.id.Value(), /*group_title=*/"Test group 1 title",
        /*thread_type=*/e8::CMTT_TEMPORAL, env.CurrentHostId(), env.DemowebDatabase());

    e8::ChatMessageGroupEntity group2 = e8::CreateChatMessageGroup(
        *creator->id.Value(), *message_channel.id.Value(), /*group_title=*/"Test group 2 title",
        /*thread_type=*/e8::CMTT_TEMPORAL, env.CurrentHostId(), env.DemowebDatabase());

    e8::ChatMessageGroupEntity group3 = e8::CreateChatMessageGroup(
        *creator->id.Value(), *message_channel.id.Value(), /*group_title=*/"Test group 3 title",
        /*thread_type=*/e8::CMTT_TEMPORAL, env.CurrentHostId(), env.DemowebDatabase());

    // Group1 will contain 1 message;
    // Group2 will contain 3 messages;
    // Group3 will contain no message
    e8::ChatMessageEntity group1_message1 = e8::CreateChatMessage(
        *group1.id.Value(), *creator->id.Value(),
        /*text_entries=*/{"g1m1"},
        /*binary_content_paths=*/std::vector<std::string>(), env.DemowebDatabase());

    e8::ChatMessageEntity group2_message1 = e8::CreateChatMessage(
        *group2.id.Value(), *creator->id.Value(),
        /*text_entries=*/{"g2m1"},
        /*binary_content_paths=*/std::vector<std::string>(), env.DemowebDatabase());
    e8::ChatMessageEntity group2_message2 = e8::CreateChatMessage(
        *group2.id.Value(), *creator->id.Value(),
        /*text_entries=*/{"g2m2"},
        /*binary_content_paths=*/std::vector<std::string>(), env.DemowebDatabase());
    e8::ChatMessageEntity group2_message3 = e8::CreateChatMessage(
        *group2.id.Value(), *creator->id.Value(),
        /*text_entries=*/{"g2m3"},
        /*binary_content_paths=*/std::vector<std::string>(), env.DemowebDatabase());

    // Read the groups back out.
    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);

    std::vector<e8::ChatMessageThread> fetched_empty_page1 =
        e8::GetChatMessageGroupsWithChatMessageSummaryList(
            *creator->id.Value(), *empty_message_channel.id.Value(),
            /*max_num_messages_per_group=*/2, page1, env.MessageChannelPbac(), env.KeyGen(),
            env.DemowebDatabase());

    TEST_CONDITION(fetched_empty_page1.empty());

    std::vector<e8::ChatMessageThread> fetched_page1 =
        e8::GetChatMessageGroupsWithChatMessageSummaryList(
            *creator->id.Value(), *message_channel.id.Value(), /*max_num_messages_per_group=*/2,
            page1, env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(fetched_page1.size() == 2);

    TEST_CONDITION(fetched_page1[0].thread_id() == *group3.id.Value());
    TEST_CONDITION(fetched_page1[0].channel_id() == *message_channel.id.Value());
    TEST_CONDITION(fetched_page1[0].thread_title() == "Test group 3 title");
    TEST_CONDITION(fetched_page1[0].messages().empty());

    TEST_CONDITION(fetched_page1[1].thread_id() == *group2.id.Value());
    TEST_CONDITION(fetched_page1[1].channel_id() == *message_channel.id.Value());
    TEST_CONDITION(fetched_page1[1].thread_title() == "Test group 2 title");

    TEST_CONDITION(fetched_page1[1].messages().size() == 2);
    TEST_CONDITION(fetched_page1[1].messages()[0].texts().size() == 1);
    TEST_CONDITION(fetched_page1[1].messages()[0].texts()[0] == "g2m2");
    TEST_CONDITION(fetched_page1[1].messages()[0].sender().user_id() == *creator->id.Value());
    TEST_CONDITION(fetched_page1[1].messages()[1].texts().size() == 1);
    TEST_CONDITION(fetched_page1[1].messages()[1].texts()[0] == "g2m3");

    e8::Pagination page2;
    page2.set_page_number(1);
    page2.set_result_per_page(2);
    std::vector<e8::ChatMessageThread> fetched_page2 =
        e8::GetChatMessageGroupsWithChatMessageSummaryList(
            *creator->id.Value(), *message_channel.id.Value(), /*max_num_messages_per_group=*/2,
            page2, env.MessageChannelPbac(), env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(fetched_page2.size() == 1);

    TEST_CONDITION(fetched_page2[0].thread_id() == *group1.id.Value());
    TEST_CONDITION(fetched_page2[0].channel_id() == *message_channel.id.Value());
    TEST_CONDITION(fetched_page2[0].thread_title() == "Test group 1 title");

    TEST_CONDITION(fetched_page2[0].messages().size() == 1);
    TEST_CONDITION(fetched_page2[0].messages()[0].texts().size() == 1);
    TEST_CONDITION(fetched_page2[0].messages()[0].texts()[0] == "g1m1");
    TEST_CONDITION(fetched_page2[0].messages()[0].sender().user_id() == *creator->id.Value());

    return true;
}

int main() {
    e8::BeginTestSuite("chat_message_group");
    e8::RunTest("CreateChatMessageGroupTest", CreateChatMessageGroupTest);
    e8::RunTest("GetChatMessageGroupsWithChatMessageSummaryListTest",
                GetChatMessageGroupsWithChatMessageSummaryListTest);
    e8::EndTestSuite();
    return 0;
}
