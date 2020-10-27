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
#include "demoweb_service/demoweb/common_entity/message_channel_has_user_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "proto_cc/message_channel.pb.h"
#include "proto_cc/pagination.pb.h"

static e8::UserId const kCreatorId = 1;
static e8::UserId const kRegularMemberId = 2;

static constexpr char const *kChannel1Name = "channel1a";
static constexpr char const *kChannel1Desc = "Description of channel1";

static constexpr char const *kChannel2Name = "channel2a";
static constexpr char const *kChannel2Desc = "Description of channel2a";

struct CreateNewChannelInfo {
    e8::UserEntity creator;
    e8::UserEntity regular_member;
    e8::MessageChannelEntity message_channel;
};

CreateNewChannelInfo CreateChannel(std::string const &channel_name, std::string const &channel_desc,
                                   e8::DemoWebTestEnvironmentContext *env) {
    std::optional<e8::UserEntity> creator = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kCreatorId, env->CurrentHostId(),
        env->DemowebDatabase());
    if (!creator.has_value()) {
        creator = e8::FetchUser(kCreatorId, env->DemowebDatabase());
    }

    std::optional<e8::UserEntity> regular_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kRegularMemberId, env->CurrentHostId(),
        env->DemowebDatabase());
    if (!regular_member.has_value()) {
        regular_member = e8::FetchUser(kRegularMemberId, env->DemowebDatabase());
    }

    std::optional<e8::MessageChannelEntity> channel = e8::CreateMessageChannel(
        channel_name, channel_desc,
        /*encrypted=*/false,
        /*close_group_channel=*/true, env->CurrentHostId(), env->DemowebDatabase());
    assert(channel.has_value());

    bool rc = e8::CreateMessageChannelMembership(*channel->id.Value(), kCreatorId, e8::MCMT_ADMIN,
                                                 env->DemowebDatabase());
    assert(rc == true);

    rc = e8::CreateMessageChannelMembership(*channel->id.Value(), kRegularMemberId, e8::MCMT_MEMBER,
                                            env->DemowebDatabase());
    assert(rc == true);

    CreateNewChannelInfo info;
    info.creator = *creator;
    info.regular_member = *regular_member;
    info.message_channel = *channel;

    return info;
}

CreateNewChannelInfo CreateChannel1(e8::DemoWebTestEnvironmentContext *env) {
    return CreateChannel(kChannel1Name, kChannel1Desc, env);
}

CreateNewChannelInfo CreateChannel2(e8::DemoWebTestEnvironmentContext *env) {
    return CreateChannel(kChannel2Name, kChannel2Desc, env);
}

bool CreateAndListMessageChannelTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channels =
        e8::SearchMessageChannels(kCreatorId, /*contains_member_ids=*/{},
                                  /*any_channel_ids=*/{}, /*search_text=*/std::nullopt,
                                  /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(*retrieved_channels[0].message_channel.id.Value() ==
                   *channel_info.message_channel.id.Value());
    TEST_CONDITION(*retrieved_channels[0].message_channel.channel_name.Value() == kChannel1Name);
    TEST_CONDITION(*retrieved_channels[0].message_channel.description.Value() == kChannel1Desc);
    TEST_CONDITION(*retrieved_channels[0].message_channel.encryption_enabled.Value() == false);
    TEST_CONDITION(*retrieved_channels[0].message_channel.close_group_channel.Value() == true);
    TEST_CONDITION(*retrieved_channels[0].message_channel.created_at.Value() > 0);
    TEST_CONDITION(retrieved_channels[0].member_type == e8::MessageChannelMemberType::MCMT_ADMIN);

    return true;
}

bool ListMessageChannelWithMemberIdsTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    std::vector<e8::SearchedMessageChannel> retrieved_channels = e8::SearchMessageChannels(
        kCreatorId,
        /*contains_member_ids=*/{}, /*any_channel_ids=*/{}, /*search_text=*/std::nullopt,
        /*active_member_fetch_limit=*/10, std::nullopt, env.DemowebDatabase());
    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(retrieved_channels[0].most_active_members.size() == 2);
    TEST_CONDITION(std::find_if(retrieved_channels[0].most_active_members.begin(),
                                retrieved_channels[0].most_active_members.end(),
                                [](e8::MessageChannelHasUserEntity const &member) {
                                    return *member.user_id.Value() == kCreatorId;
                                }) != retrieved_channels[0].most_active_members.end());
    TEST_CONDITION(std::find_if(retrieved_channels[0].most_active_members.begin(),
                                retrieved_channels[0].most_active_members.end(),
                                [](e8::MessageChannelHasUserEntity const &member) {
                                    return *member.user_id.Value() == kRegularMemberId;
                                }) != retrieved_channels[0].most_active_members.end());

    retrieved_channels = e8::SearchMessageChannels(
        kCreatorId, /*contains_member_ids=*/{}, /*any_channel_ids=*/{},
        /*search_text=*/std::nullopt,
        /*active_member_fetch_limit=*/1, std::nullopt, env.DemowebDatabase());
    TEST_CONDITION(retrieved_channels.size() == 1);
    TEST_CONDITION(retrieved_channels[0].most_active_members.size() == 1);

    return true;
}

bool ListMessageChannelMemberFilterTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channel = e8::SearchMessageChannels(
        kCreatorId,
        /*contains_member_ids=*/{}, /*any_channel_ids=*/{}, /*search_text=*/std::nullopt,
        /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channel.size() == 1);
    TEST_CONDITION(*retrieved_channel[0].message_channel.id.Value() ==
                   *channel_info.message_channel.id.Value());

    retrieved_channel =
        e8::SearchMessageChannels(kCreatorId,
                                  /*contains_member_ids=*/{kCreatorId, -1000L},
                                  /*any_channel_ids=*/{}, /*search_text=*/std::nullopt,
                                  /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());
    TEST_CONDITION(retrieved_channel.empty());

    return true;
}

bool ListMessageChannelIdFilterTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel1 = CreateChannel1(&env);
    CreateNewChannelInfo channel2 = CreateChannel2(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channel = e8::SearchMessageChannels(
        kCreatorId,
        /*contains_member_ids=*/{}, /*any_channel_ids=*/{*channel1.message_channel.id.Value()},
        /*search_text=*/std::nullopt,
        /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channel.size() == 1);
    TEST_CONDITION(*retrieved_channel[0].message_channel.id.Value() ==
                   *channel1.message_channel.id.Value());

    return true;
}

bool ListMessageChannelQueryTextFilterTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel1 = CreateChannel1(&env);
    CreateNewChannelInfo channel2 = CreateChannel2(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::SearchedMessageChannel> retrieved_channel =
        e8::SearchMessageChannels(kCreatorId,
                                  /*contains_member_ids=*/{}, /*any_channel_ids=*/{},
                                  /*search_text=*/"channel1",
                                  /*active_member_fetch_limit=*/0, page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_channel.size() == 1);
    TEST_CONDITION(*retrieved_channel[0].message_channel.id.Value() ==
                   *channel1.message_channel.id.Value());

    return true;
}

bool CreateAndListChannelMemberTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(3);
    std::vector<e8::MessageChannelMember> retrieved_members = e8::GetMessageChannelMembers(
        *channel_info.message_channel.id.Value(), page1, env.DemowebDatabase());

    TEST_CONDITION(retrieved_members.size() == 2);

    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == kCreatorId &&
                                           member.member_type ==
                                               e8::MessageChannelMemberType::MCMT_ADMIN &&
                                           member.join_at > 0;
                                }) != retrieved_members.end());

    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == kRegularMemberId &&
                                           member.member_type ==
                                               e8::MessageChannelMemberType::MCMT_MEMBER &&
                                           member.join_at > 0;
                                }) != retrieved_members.end());

    return true;
}

bool UpdateMessageChannelMembershipTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    std::optional<e8::UserEntity> new_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/100L, env.CurrentHostId(),
        env.DemowebDatabase());

    // Membership proposal.
    e8::MessageChannelMembership unchanged_creator;
    unchanged_creator.set_channel_id(*channel_info.message_channel.id.Value());
    unchanged_creator.set_user_id(kCreatorId);
    unchanged_creator.set_member_type(e8::MCMT_ADMIN);

    e8::MessageChannelMembership new_membership;
    new_membership.set_channel_id(*channel_info.message_channel.id.Value());
    new_membership.set_user_id(*new_member->id.Value());
    new_membership.set_member_type(e8::MCMT_ADMIN);

    e8::MessageChannelMembership promote_regular_member;
    promote_regular_member.set_channel_id(*channel_info.message_channel.id.Value());
    promote_regular_member.set_user_id(kRegularMemberId);
    promote_regular_member.set_member_type(e8::MCMT_ADMIN);

    // Apply the proposal.
    bool rc = e8::UpdateMessageChannelMembership(
        /*viewer_id=*/kCreatorId, *channel_info.message_channel.id.Value(),
        {unchanged_creator, new_membership, promote_regular_member}, env.MessageChannelPbac(),
        env.DemowebDatabase());
    TEST_CONDITION(rc == true);

    std::vector<e8::MessageChannelMember> retrieved_members =
        e8::GetMessageChannelMembers(*channel_info.message_channel.id.Value(),
                                     /*pagination=*/std::nullopt, env.DemowebDatabase());
    TEST_CONDITION(retrieved_members.size() == 3);

    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == kCreatorId &&
                                           member.member_type == e8::MCMT_ADMIN;
                                }) != retrieved_members.end());

    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == kRegularMemberId &&
                                           member.member_type == e8::MCMT_ADMIN;
                                }) != retrieved_members.end());

    TEST_CONDITION(std::find_if(retrieved_members.begin(), retrieved_members.end(),
                                [](e8::MessageChannelMember const &member) {
                                    return *member.member.id.Value() == 100L &&
                                           member.member_type == e8::MCMT_ADMIN;
                                }) != retrieved_members.end());

    return true;
}

bool UpdateMessageChannelMetadataTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    std::optional<e8::MessageChannelEntity> updated = e8::UpdateMessageChannelMetadata(
        kRegularMemberId, *channel_info.message_channel.id.Value(),
        /*channel_name=*/"new channel name",
        /*description=*/"new description", env.MessageChannelPbac(), env.DemowebDatabase());

    TEST_CONDITION(updated.has_value());
    TEST_CONDITION(*updated->id.Value() == *channel_info.message_channel.id.Value());
    TEST_CONDITION(*updated->encryption_enabled.Value() ==
                   *channel_info.message_channel.encryption_enabled.Value());
    TEST_CONDITION(*updated->close_group_channel.Value() ==
                   *channel_info.message_channel.close_group_channel.Value());
    TEST_CONDITION(*updated->channel_name.Value() == "new channel name");
    TEST_CONDITION(*updated->description.Value() == "new description");

    return true;
}

bool ToMessageChannelOverviewsTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    std::vector<e8::SearchedMessageChannel> retrieved_channels = e8::SearchMessageChannels(
        kCreatorId,
        /*contains_member_ids=*/{}, /*any_channel_ids=*/{}, /*search_text=*/std::nullopt,
        /*active_member_fetch_limit=*/10, std::nullopt, env.DemowebDatabase());

    std::vector<e8::MessageChannelOverview> overviews = e8::ToMessageChannelOverviews(
        kCreatorId, retrieved_channels, env.KeyGen(), env.DemowebDatabase());

    TEST_CONDITION(overviews.size() == 1);
    TEST_CONDITION(overviews[0].channel().channel_id() == *channel_info.message_channel.id.Value());
    TEST_CONDITION(overviews[0].channel().created_at() > 0);
    TEST_CONDITION(overviews[0].channel_last_interacted_at() > 0);
    TEST_CONDITION(overviews[0].most_active_users().size() == 2);

    TEST_CONDITION(std::find_if(overviews[0].most_active_users().begin(),
                                overviews[0].most_active_users().end(),
                                [](e8::UserPublicProfile const &member) {
                                    return member.user_id() == kCreatorId;
                                }) != overviews[0].most_active_users().end());

    TEST_CONDITION(std::find_if(overviews[0].most_active_users().begin(),
                                overviews[0].most_active_users().end(),
                                [](e8::UserPublicProfile const &member) {
                                    return member.user_id() == kRegularMemberId;
                                }) != overviews[0].most_active_users().end());

    return true;
}

bool MessageChannelMembershipTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    bool should_exist = e8::UserInMessageChannel(
        kCreatorId, *channel_info.message_channel.id.Value(), env.DemowebDatabase());
    TEST_CONDITION(should_exist);

    bool should_not_exist = !e8::UserInMessageChannel(
        /*user_id*/ 123456L, *channel_info.message_channel.id.Value(), env.DemowebDatabase());
    TEST_CONDITION(should_not_exist);

    return true;
}

bool ComputeMessageChannelMembershipDeltaTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel1(&env);

    // Current memberships.
    e8::MessageChannelMembership creator;
    creator.set_user_id(kCreatorId);
    creator.set_channel_id(*channel_info.message_channel.id.Value());
    creator.set_member_type(e8::MCMT_ADMIN);

    e8::MessageChannelMembership regular_member;
    regular_member.set_user_id(kRegularMemberId);
    regular_member.set_channel_id(*channel_info.message_channel.id.Value());
    regular_member.set_member_type(e8::MCMT_MEMBER);

    // Removal.
    std::vector<e8::MessageChannelMembership> proposed_removal{creator};
    e8::MessageChannelMembershipDelta delta =
        e8::message_channel_internal::ComputeMessageChannelMembershipDelta(
            *channel_info.message_channel.id.Value(), proposed_removal, env.DemowebDatabase());
    TEST_CONDITION(delta.to_be_added.empty());
    TEST_CONDITION(delta.to_be_modified.empty());
    TEST_CONDITION(delta.to_be_removed.size() == 1);
    TEST_CONDITION(delta.to_be_removed[0].user_id() == kRegularMemberId);

    // Addition.
    e8::MessageChannelMembership new_member;
    new_member.set_user_id(100L);
    new_member.set_channel_id(*channel_info.message_channel.id.Value());
    new_member.set_member_type(e8::MCMT_ADMIN);

    std::vector<e8::MessageChannelMembership> proposed_addition{creator, regular_member,
                                                                new_member};
    delta = e8::message_channel_internal::ComputeMessageChannelMembershipDelta(
        *channel_info.message_channel.id.Value(), proposed_addition, env.DemowebDatabase());
    TEST_CONDITION(delta.to_be_modified.empty());
    TEST_CONDITION(delta.to_be_removed.empty());
    TEST_CONDITION(delta.to_be_added.size() == 1);
    TEST_CONDITION(delta.to_be_added[0].user_id() == 100L);
    TEST_CONDITION(delta.to_be_added[0].member_type() == e8::MCMT_ADMIN);

    // Modification.
    regular_member.set_member_type(e8::MCMT_ADMIN);
    std::vector<e8::MessageChannelMembership> proposed_modification{creator, regular_member};
    delta = e8::message_channel_internal::ComputeMessageChannelMembershipDelta(
        *channel_info.message_channel.id.Value(), proposed_modification, env.DemowebDatabase());
    TEST_CONDITION(delta.to_be_removed.empty());
    TEST_CONDITION(delta.to_be_added.empty());
    TEST_CONDITION(delta.to_be_modified.size() == 1);
    TEST_CONDITION(delta.to_be_modified[0].user_id() == kRegularMemberId);
    TEST_CONDITION(delta.to_be_modified[0].member_type() == e8::MCMT_ADMIN);

    return true;
}

int main() {
    e8::BeginTestSuite("message_channel");
    e8::RunTest("CreateAndListMessageChannelTest", CreateAndListMessageChannelTest);
    e8::RunTest("ListMessageChannelMemberFilterTest", ListMessageChannelMemberFilterTest);
    e8::RunTest("ListMessageChannelIdFilterTest", ListMessageChannelIdFilterTest);
    e8::RunTest("ListMessageChannelQueryTextFilterTest", ListMessageChannelQueryTextFilterTest);
    e8::RunTest("ListMessageChannelWithMemberIdsTest", ListMessageChannelWithMemberIdsTest);
    e8::RunTest("CreateAndListChannelMemberTest", CreateAndListChannelMemberTest);
    e8::RunTest("UpdateMessageChannelMembershipTest", UpdateMessageChannelMembershipTest);
    e8::RunTest("UpdateMessageChannelMetadataTest", UpdateMessageChannelMetadataTest);
    e8::RunTest("ToMessageChannelOverviewsTest", ToMessageChannelOverviewsTest);
    e8::RunTest("MessageChannelMembershipTest", MessageChannelMembershipTest);
    e8::RunTest("ComputeMessageChannelMembershipDeltaTest",
                ComputeMessageChannelMembershipDeltaTest);
    e8::EndTestSuite();
    return 0;
}
