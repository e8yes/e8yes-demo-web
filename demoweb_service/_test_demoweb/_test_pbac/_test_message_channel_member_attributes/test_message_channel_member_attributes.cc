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
#include <unordered_map>

#include "common/unit_test_util/unit_test_util.h"
#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/module/user_storage.h"
#include "demoweb_service/demoweb/pbac/message_channel_member_attributes.h"
#include "proto_cc/message_channel.pb.h"

static e8::UserId const kCreatorId = 1;

static constexpr char const *kChannelName = "channel1a";
static constexpr char const *kChannelDesc = "Description of channel1";

struct CreateNewChannelInfo {
    e8::UserEntity creator;
    e8::MessageChannelEntity message_channel;
};

CreateNewChannelInfo CreateChannel(e8::DemoWebTestEnvironmentContext *env) {
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

bool ExtractAllMessageChannelMemberAttributesTest() {
    e8::DemoWebTestEnvironmentContext env;
    CreateNewChannelInfo channel = CreateChannel(&env);

    std::unordered_map<e8::UserId, e8::MessageChannelMemberAttributes> attrs =
        e8::ExtractMessageChannelMemberAttributes(*channel.message_channel.id.Value(),
                                                  env.DemowebDatabase());

    TEST_CONDITION(attrs.size() == 1);

    auto it = attrs.find(kCreatorId);
    TEST_CONDITION(it != attrs.end());
    TEST_CONDITION(it->second.member_type == e8::MCMT_ADMIN);

    return true;
}

bool ExtractAllNonExistingMessageChannelMemberAttributesTest() {
    e8::DemoWebTestEnvironmentContext env;
    std::unordered_map<e8::UserId, e8::MessageChannelMemberAttributes> attrs =
        e8::ExtractMessageChannelMemberAttributes(/*channel_id=*/1L, env.DemowebDatabase());
    TEST_CONDITION(attrs.empty());
    return true;
}

bool ExtractOneMessageChannelMemberAttributesTest() {
    e8::DemoWebTestEnvironmentContext env;
    CreateNewChannelInfo channel = CreateChannel(&env);

    std::optional<e8::MessageChannelMemberAttributes> attrs =
        e8::ExtractMessageChannelMemberAttributes(*channel.message_channel.id.Value(), kCreatorId,
                                                  env.DemowebDatabase());

    TEST_CONDITION(attrs.has_value());
    TEST_CONDITION(attrs->member_type == e8::MCMT_ADMIN);

    return true;
}

bool ExtractOneNonExistingMessageChannelMemberAttributesTest() {
    e8::DemoWebTestEnvironmentContext env;
    std::optional<e8::MessageChannelMemberAttributes> attrs =
        e8::ExtractMessageChannelMemberAttributes(/*channel_id=*/1L, /*user_id=*/1L,
                                                  env.DemowebDatabase());
    TEST_CONDITION(!attrs.has_value());
    return true;
}

int main() {
    e8::BeginTestSuite("message_channel_member_attributes");
    e8::RunTest("ExtractAllMessageChannelMemberAttributesTest",
                ExtractAllMessageChannelMemberAttributesTest);
    e8::RunTest("ExtractAllNonExistingMessageChannelMemberAttributesTest",
                ExtractAllNonExistingMessageChannelMemberAttributesTest);
    e8::RunTest("ExtractOneMessageChannelMemberAttributesTest",
                ExtractOneMessageChannelMemberAttributesTest);
    e8::RunTest("ExtractOneNonExistingMessageChannelMemberAttributesTest",
                ExtractOneNonExistingMessageChannelMemberAttributesTest);
    e8::EndTestSuite();
    return 0;
}
