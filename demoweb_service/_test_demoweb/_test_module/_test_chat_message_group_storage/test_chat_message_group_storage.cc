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
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/chat_message_group_storage.h"
#include "demoweb_service/demoweb/module/message_channel_storage.h"
#include "demoweb_service/demoweb/module/user_storage.h"

bool CreateAndFetchChatMessageGroupTest() {
    e8::DemoWebTestEnvironmentContext env;

    std::optional<e8::UserEntity> user =
        e8::CreateUser(/*security_key=*/"", /*user_group_names=*/std::vector<std::string>(),
                       /*user_id=*/1L, /*host_id=*/env.CurrentHostId(), env.DemowebDatabase());

    e8::MessageChannelEntity channel = e8::CreateMessageChannel(
        /*channel_name=*/std::nullopt, /*description=*/std::nullopt, /*encrypted=*/false,
        /*close_group_channel=*/false, env.CurrentHostId(), env.DemowebDatabase());

    e8::ChatMessageGroupEntity group =
        e8::CreateChatMessageGroup(*user->id.Value(), *channel.id.Value(),
                                   /*group_title=*/"Test chat message group", e8::CMTT_POPUP,
                                   env.CurrentHostId(), env.DemowebDatabase());
    std::optional<e8::ChatMessageGroupEntity> fetched =
        e8::FetchChatMessageGroup(*group.id.Value(), env.DemowebDatabase());

    TEST_CONDITION(fetched.has_value());
    TEST_CONDITION(*fetched->id.Value() == *group.id.Value());
    TEST_CONDITION(*fetched->channel_id.Value() == *channel.id.Value());
    TEST_CONDITION(*fetched->creator_id.Value() == *user->id.Value());
    TEST_CONDITION(*fetched->description.Value() == "Test chat message group");
    TEST_CONDITION(*fetched->created_at.Value() > 0);
    TEST_CONDITION(*fetched->last_interaction_at.Value() > 0);

    return true;
}

int main() {
    e8::BeginTestSuite("chat_message_group_storage");
    e8::RunTest("CreateAndFetchChatMessageGroupTest", CreateAndFetchChatMessageGroupTest);
    e8::EndTestSuite();
    return 0;
}
