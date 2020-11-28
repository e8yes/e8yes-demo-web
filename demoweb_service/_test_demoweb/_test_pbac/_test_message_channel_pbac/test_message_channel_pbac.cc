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

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"

static e8::UserId const kCreatorId = 1;
static e8::UserId const kAdminMemberId = 2;
static e8::UserId const kRegularMemberId = 3;

static constexpr char const *kChannelName = "channel1a";
static constexpr char const *kChannelDesc = "Description of channel1";

struct CreateNewChannelInfo {
    e8::UserEntity creator;
    e8::UserEntity admin_member;
    e8::UserEntity regular_member;
    e8::MessageChannelEntity message_channel;
};

CreateNewChannelInfo CreateChannel(bool close_group_channel,
                                   e8::DemoWebTestEnvironmentContext *env) {
    std::optional<e8::UserEntity> creator = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kCreatorId, env->CurrentHostId(),
        env->DemowebDatabase());

    std::optional<e8::UserEntity> admin_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kAdminMemberId, env->CurrentHostId(),
        env->DemowebDatabase());

    std::optional<e8::UserEntity> regular_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), kRegularMemberId, env->CurrentHostId(),
        env->DemowebDatabase());

    std::optional<e8::MessageChannelEntity> channel = e8::CreateMessageChannel(
        kChannelName, kChannelDesc,
        /*encrypted=*/false, close_group_channel, env->CurrentHostId(), env->DemowebDatabase());
    assert(channel.has_value());

    e8::CreateMessageChannelMembership(*channel->id.Value(), kCreatorId, e8::MCMT_ADMIN,
                                       env->DemowebDatabase());
    e8::CreateMessageChannelMembership(*channel->id.Value(), kAdminMemberId, e8::MCMT_ADMIN,
                                       env->DemowebDatabase());
    e8::CreateMessageChannelMembership(*channel->id.Value(), kRegularMemberId, e8::MCMT_MEMBER,
                                       env->DemowebDatabase());

    CreateNewChannelInfo info;
    info.creator = *creator;
    info.admin_member = *admin_member;
    info.regular_member = *regular_member;
    info.message_channel = *channel;
    return info;
}

bool AllowUpdateCloseGroupChannelMetadataTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/true, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator.
    bool should_allow = pbac.AllowUpdateChannelMetadata(/*operator_user_id=*/kCreatorId,
                                                        *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Regular channel member operator.
    should_allow = pbac.AllowUpdateChannelMetadata(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Non-channel-member operator.
    bool should_not_allow = !pbac.AllowUpdateChannelMetadata(
        /*operator_user_id=*/1000L, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowUpdateOpenGroupChannelMetadataTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator.
    bool should_allow = pbac.AllowUpdateChannelMetadata(/*operator_user_id=*/kCreatorId,
                                                        *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Regular channel member operator.
    bool should_not_allow = !pbac.AllowUpdateChannelMetadata(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Non-channel-member operator.
    should_not_allow = !pbac.AllowUpdateChannelMetadata(
        /*operator_user_id=*/3L, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowDeleteChannelTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/true, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator.
    bool should_allow = pbac.AllowDeleteChannel(/*operator_user_id=*/kCreatorId,
                                                *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Regular member operator.
    bool should_not_allow = !pbac.AllowDeleteChannel(/*operator_user_id=*/kRegularMemberId,
                                                     *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowUpdateCloseGroupChannelMembershipTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/true, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator adding a user as a new admin.
    std::optional<e8::UserEntity> new_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/10L, env.CurrentHostId(),
        env.DemowebDatabase());

    bool should_allow = pbac.AllowUpdateChannelMembership(/*operator_user_id=*/kCreatorId,
                                                          *channel_info.message_channel.id.Value(),
                                                          *new_member->id.Value(), e8::MCMT_ADMIN);
    TEST_CONDITION(should_allow);

    // Regular member operator adding a user as a new admin.
    bool should_not_allow = !pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        *new_member->id.Value(), e8::MCMT_ADMIN);
    TEST_CONDITION(should_not_allow);

    // Regular member operator adding a user as a new member.
    should_allow = pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        *new_member->id.Value(), e8::MCMT_MEMBER);
    TEST_CONDITION(should_allow);

    return true;
}

bool AllowUpdateOpenGroupChannelMembershipTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator adding a user as a new admin.
    std::optional<e8::UserEntity> new_member = e8::CreateUser(
        /*security_key=*/"", std::vector<std::string>(), /*user_id=*/10L, env.CurrentHostId(),
        env.DemowebDatabase());

    bool should_allow = pbac.AllowUpdateChannelMembership(/*operator_user_id=*/kCreatorId,
                                                          *channel_info.message_channel.id.Value(),
                                                          *new_member->id.Value(), e8::MCMT_ADMIN);
    TEST_CONDITION(should_allow);

    // Regular member operator adding a user as a new admin.
    bool should_not_allow = !pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        *new_member->id.Value(), e8::MCMT_MEMBER);
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowUpdateChannelMembershipDemotionTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/true, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Demote by an admin.
    bool should_allow = pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kAdminMemberId, *channel_info.message_channel.id.Value(),
        /*user_to_be_updated=*/kCreatorId, e8::MCMT_MEMBER);
    TEST_CONDITION(should_allow);

    // Demote by a regular member.
    bool should_not_allow = !pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        /*user_to_be_updated=*/kCreatorId, e8::MCMT_MEMBER);
    TEST_CONDITION(should_not_allow);

    // Demote the only admin.
    e8::UpdateMessageChannelMembership(*channel_info.message_channel.id.Value(), kAdminMemberId,
                                       e8::MCMT_MEMBER, env.DemowebDatabase());
    should_not_allow = !pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kCreatorId, *channel_info.message_channel.id.Value(),
        /*user_to_be_updated=*/kCreatorId, e8::MCMT_MEMBER);
    TEST_CONDITION(should_allow);

    return true;
}

bool AllowUpdateChannelMembershipPromotionTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/true, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator.
    bool should_allow = pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kCreatorId, *channel_info.message_channel.id.Value(),
        /*user_to_be_updated=*/kRegularMemberId, e8::MCMT_ADMIN);
    TEST_CONDITION(should_allow);

    // Regular member operator.
    bool should_not_allow = !pbac.AllowUpdateChannelMembership(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        /*user_to_be_updated=*/kRegularMemberId, e8::MCMT_ADMIN);
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowDeleteMemberFromChannelTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Admin operator removing a regular member.
    bool should_allow = pbac.AllowDeleteMemberFromChannel(/*operator_user_id=*/kCreatorId,
                                                          *channel_info.message_channel.id.Value(),
                                                          /*user_to_be_removed=*/kRegularMemberId);
    TEST_CONDITION(should_allow);

    // Regular member operator removing admin.
    bool should_not_allow = !pbac.AllowDeleteMemberFromChannel(
        /*operator_user_id=*/kRegularMemberId, *channel_info.message_channel.id.Value(),
        /*user_to_be_removed=*/kAdminMemberId);
    TEST_CONDITION(should_not_allow);

    // Regular member operator removing himself.
    should_allow = pbac.AllowDeleteMemberFromChannel(/*operator_user_id=*/kRegularMemberId,
                                                     *channel_info.message_channel.id.Value(),
                                                     /*user_to_be_removed=*/kRegularMemberId);
    TEST_CONDITION(should_allow);

    // The only admin removing himself.
    e8::UpdateMessageChannelMembership(*channel_info.message_channel.id.Value(), kAdminMemberId,
                                       e8::MCMT_MEMBER, env.DemowebDatabase());
    should_not_allow = !pbac.AllowDeleteMemberFromChannel(
        /*operator_user_id=*/kCreatorId, *channel_info.message_channel.id.Value(),
        /*user_to_be_removed=*/kCreatorId);
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowCreateChatMessageGroupTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Channel member creating a chat message group.
    bool should_allow = pbac.AllowCreateChatMessageGroup(/*operator_user_id=*/kRegularMemberId,
                                                         *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Non-member creating a chat message group.
    bool should_not_allow = !pbac.AllowCreateChatMessageGroup(
        /*operator_user_id=*/10000L, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowReadChatMessageGroupTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Channel member creating a chat message group.
    bool should_allow = pbac.AllowReadChatMessageGroup(/*operator_user_id=*/kRegularMemberId,
                                                       *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Non-member creating a chat message group.
    bool should_not_allow = !pbac.AllowCreateChatMessageGroup(
        /*operator_user_id=*/10000L, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

bool AllowSendChatMessageTest() {
    e8::DemoWebTestEnvironmentContext env;

    CreateNewChannelInfo channel_info = CreateChannel(/*close_group_channel=*/false, &env);

    e8::MessageChannelPbacImpl pbac(env.DemowebDatabase());

    // Channel member creating a chat message group.
    bool should_allow = pbac.AllowSendChatMessage(/*operator_user_id=*/kRegularMemberId,
                                                  *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_allow);

    // Non-member creating a chat message group.
    bool should_not_allow = !pbac.AllowSendChatMessage(
        /*operator_user_id=*/10000L, *channel_info.message_channel.id.Value());
    TEST_CONDITION(should_not_allow);

    return true;
}

int main() {
    e8::BeginTestSuite("message_channel_pbac");
    e8::RunTest("AllowUpdateCloseGroupChannelMetadataTest",
                AllowUpdateCloseGroupChannelMetadataTest);
    e8::RunTest("AllowUpdateOpenGroupChannelMetadataTest", AllowUpdateOpenGroupChannelMetadataTest);
    e8::RunTest("AllowDeleteChannelTest", AllowDeleteChannelTest);
    e8::RunTest("AllowUpdateChannelMembershipTest", AllowUpdateCloseGroupChannelMembershipTest);
    e8::RunTest("AllowUpdateOpenGroupChannelMembershipTest",
                AllowUpdateOpenGroupChannelMembershipTest);
    e8::RunTest("AllowUpdateChannelMembershipDemotionTest",
                AllowUpdateChannelMembershipDemotionTest);
    e8::RunTest("AllowUpdateChannelMembershipPromotionTest",
                AllowUpdateChannelMembershipPromotionTest);
    e8::RunTest("AllowDeleteMemberFromChannelTest", AllowDeleteMemberFromChannelTest);
    e8::RunTest("AllowCreateChatMessageGroupTest", AllowCreateChatMessageGroupTest);
    e8::RunTest("AllowReadChatMessageGroupTest", AllowReadChatMessageGroupTest);
    e8::RunTest("AllowSendChatMessageTest", AllowSendChatMessageTest);
    e8::EndTestSuite();
    return 0;
}
