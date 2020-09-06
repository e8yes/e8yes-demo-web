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
#include <optional>
#include <string>
#include <vector>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "proto_cc/message_channel.pb.h"
#include "proto_cc/pagination.pb.h"

static e8::UserId const kCreatorId = 1;
static constexpr char const *kChannelName = "channel1";
static constexpr char const *kChannelDesc = "Description of channel1";

struct CreateNewChannelInfo {
    e8::UserEntity creator;
    e8::MessageChannelEntity message_channel;
};

CreateNewChannelInfo CreateNewChannel(e8::DemoWebTestEnvironmentContext *env) {
    std::optional<e8::UserEntity> user = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kCreatorId, env->CurrentHostId(),
        env->DemowebDatabase());

    std::optional<e8::MessageChannelEntity> channel = e8::CreateMessageChannel(
        kCreatorId, kChannelName, kChannelDesc, /*to_be_member_ids=*/std::vector<e8::UserId>(),
        /*encrypted=*/false,
        /*close_group_channel=*/true, env->CurrentHostId(), env->DemowebDatabase());
    assert(channel.has_value());

    CreateNewChannelInfo info;
    info.creator = *user;
    info.message_channel = *channel;
    return info;
}

bool CreateAndListMessageChannelTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channels =
        e8::SearchMessageChannels(/*contains_member_ids=*/{kCreatorId},
                                  /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(*retrieved_channels[0].message_channel.id.Value() ==
                   *channel_info.message_channel.id.Value());
    TEST_CONDITION(*retrieved_channels[0].message_channel.channel_name.Value() == kChannelName);
    TEST_CONDITION(*retrieved_channels[0].message_channel.description.Value() == kChannelDesc);
    TEST_CONDITION(*retrieved_channels[0].message_channel.encryption_enabled.Value() == false);
    TEST_CONDITION(*retrieved_channels[0].message_channel.close_group_channel.Value() == true);
    TEST_CONDITION(retrieved_channels[0].member_type == e8::MessageChannelMemberType::MCMT_ADMIN);

    return true;
}

bool ListMessageChannelWithMemberIdsTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    std::optional<e8::UserEntity> user = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/2L, env.CurrentHostId(),
        env.DemowebDatabase());

    bool rc =
        e8::AddUserToMessageChannel(/*viewer_id=*/1L, *channel_info.message_channel.id.Value(),
                                    *user->id.Value(), e8::MCMT_ADMIN, env.DemowebDatabase());
    TEST_CONDITION(rc == true);

    std::vector<e8::SearchedMessageChannel> retrieved_channels = e8::SearchMessageChannels(
        /*contains_member_ids=*/{kCreatorId}, /*active_member_fetch_limit=*/10, std::nullopt,
        env.DemowebDatabase());
    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(retrieved_channels[0].most_active_member_ids.size() == 2);
    TEST_CONDITION(std::find(retrieved_channels[0].most_active_member_ids.begin(),
                             retrieved_channels[0].most_active_member_ids.end(),
                             kCreatorId) != retrieved_channels[0].most_active_member_ids.end());
    TEST_CONDITION(std::find(retrieved_channels[0].most_active_member_ids.begin(),
                             retrieved_channels[0].most_active_member_ids.end(),
                             2L) != retrieved_channels[0].most_active_member_ids.end());

    retrieved_channels = e8::SearchMessageChannels(/*contains_member_ids=*/{kCreatorId},
                                                   /*active_member_fetch_limit=*/1, std::nullopt,
                                                   env.DemowebDatabase());
    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(retrieved_channels[0].most_active_member_ids.size() == 1);

    return true;
}

bool ListMessageChannelMemberFilterTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channel = e8::SearchMessageChannels(
        /*contains_member_ids=*/{kCreatorId},
        /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channel.size() == 1);
    TEST_CONDITION(*retrieved_channel[0].message_channel.id.Value() ==
                   *channel_info.message_channel.id.Value());

    retrieved_channel = e8::SearchMessageChannels(
        /*contains_member_ids=*/{kCreatorId, -1000L},
        /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());
    TEST_CONDITION(retrieved_channel.empty());

    return true;
}

bool CreateAndListChannelMemberTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::MessageChannelMember> retrieved_members = e8::GetMessageChannelMembers(
        *channel_info.message_channel.id.Value(), page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_members.size() == 1);
    TEST_CONDITION(*retrieved_members[0].member.id.Value() == kCreatorId);
    TEST_CONDITION(retrieved_members[0].member_type == e8::MessageChannelMemberType::MCMT_ADMIN);
    TEST_CONDITION(retrieved_members[0].join_at > 0);

    return true;
}

bool AddUserToMessageChannelTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    std::optional<e8::UserEntity> user = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/2L, env.CurrentHostId(),
        env.DemowebDatabase());

    bool rc =
        e8::AddUserToMessageChannel(/*viewer_id=*/1L, *channel_info.message_channel.id.Value(),
                                    *user->id.Value(), e8::MCMT_ADMIN, env.DemowebDatabase());
    TEST_CONDITION(rc == true);

    std::vector<e8::MessageChannelMember> retrieved_members =
        e8::GetMessageChannelMembers(*channel_info.message_channel.id.Value(),
                                     /*pagination=*/std::nullopt, env.DemowebDatabase());
    TEST_CONDITION(retrieved_members.size() == 2);
    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == kCreatorId;
                                }) != retrieved_members.end());
    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == 2L;
                                }) != retrieved_members.end());

    return true;
}

bool AddUserToMessageChannelInsufficientPrivilegeTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    std::optional<e8::UserEntity> user = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/2L, env.CurrentHostId(),
        env.DemowebDatabase());
    std::optional<e8::UserEntity> user2 = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/3L, env.CurrentHostId(),
        env.DemowebDatabase());

    bool rc =
        e8::AddUserToMessageChannel(/*viewer_id=*/1L, *channel_info.message_channel.id.Value(),
                                    *user->id.Value(), e8::MCMT_MEMBER, env.DemowebDatabase());
    TEST_CONDITION(rc == true);

    rc = e8::AddUserToMessageChannel(/*viewer_id=*/2L, *channel_info.message_channel.id.Value(),
                                     *user2->id.Value(), e8::MCMT_ADMIN, env.DemowebDatabase());
    TEST_CONDITION(rc == false);

    std::vector<e8::MessageChannelMember> retrieved_members =
        e8::GetMessageChannelMembers(*channel_info.message_channel.id.Value(),
                                     /*pagination=*/std::nullopt, env.DemowebDatabase());
    TEST_CONDITION(retrieved_members.size() == 2);

    return true;
}

bool ToMessageChannelOverviewsTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateNewChannel(&env);

    std::vector<e8::SearchedMessageChannel> retrieved_channels = e8::SearchMessageChannels(
        /*contains_member_ids=*/{kCreatorId}, /*active_member_fetch_limit=*/10, std::nullopt,
        env.DemowebDatabase());

    std::vector<e8::MessageChannelOveriew> overviews = e8::ToMessageChannelOverviews(
        kCreatorId, retrieved_channels, env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(overviews.size() == 1);
    TEST_CONDITION(overviews[0].channel().channel_id() == *channel_info.message_channel.id.Value());
    TEST_CONDITION(overviews[0].channel().created_at() > 0);
    TEST_CONDITION(overviews[0].channel_last_interacted_at() > 0);
    TEST_CONDITION(overviews[0].most_active_users().size() == 1);
    TEST_CONDITION(overviews[0].most_active_users()[0].user_id() == kCreatorId);

    return true;
}

int main() {
    e8::BeginTestSuite("message_channel");
    e8::RunTest("CreateAndListMessageChannelTest", CreateAndListMessageChannelTest);
    e8::RunTest("ListMessageChannelMemberFilterTest", ListMessageChannelMemberFilterTest);
    e8::RunTest("ListMessageChannelWithMemberIdsTest", ListMessageChannelWithMemberIdsTest);
    e8::RunTest("CreateAndListChannelMemberTest", CreateAndListChannelMemberTest);
    e8::RunTest("AddUserToMessageChannelTest", AddUserToMessageChannelTest);
    e8::RunTest("AddUserToMessageChannelInsufficientPrivilegeTest",
                AddUserToMessageChannelInsufficientPrivilegeTest);
    e8::RunTest("ToMessageChannelOverviewsTest", ToMessageChannelOverviewsTest);
    e8::EndTestSuite();
    return 0;
}
