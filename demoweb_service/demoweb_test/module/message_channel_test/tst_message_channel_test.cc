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

#include <QtTest>
#include <optional>
#include <string>
#include <vector>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/environment/test_environment_context.h"
#include "demoweb_service/demoweb/module/create_user.h"
#include "demoweb_service/demoweb/module/message_channel.h"
#include "demoweb_service/demoweb/proto_cc/message_channel.pb.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"

class message_channel_test : public QObject {
    Q_OBJECT

  public:
    message_channel_test();
    ~message_channel_test();

  private slots:
    void create_and_list_message_channel_test();
};

message_channel_test::message_channel_test() {}

message_channel_test::~message_channel_test() {}

void message_channel_test::create_and_list_message_channel_test() {
    e8::TestEnvironmentContext env;

    e8::UserId user_id = 1;
    std::optional<e8::UserEntity> user =
        e8::CreateUser(/*security_key=*/"", std::vector<std::string>(), user_id,
                       env.CurrentHostId(), env.DemowebDatabase());

    std::string channel_name = "channel1";
    std::string channel_desc = "Description of channel1";
    e8::MessageChannelEntity channel = e8::CreateMessageChannel(
        user_id, channel_name, channel_desc, /*encrypted=*/false, /*close_group_channel=*/true,
        env.CurrentHostId(), env.DemowebDatabase());

    e8::Pagination page1;
    page1.set_page_number(0);
    page1.set_result_per_page(2);
    std::vector<e8::JoinedInMessageChannel> retrieved_channel =
        e8::GetJoinedInMessageChannels(user_id, page1, env.DemowebDatabase());

    QVERIFY(retrieved_channel.size() == 1);
    QVERIFY(*retrieved_channel[0].message_channel.id.Value() == *channel.id.Value());
    QVERIFY(*retrieved_channel[0].message_channel.channel_name.Value() == channel_name);
    QVERIFY(*retrieved_channel[0].message_channel.description.Value() == channel_desc);
    QVERIFY(*retrieved_channel[0].message_channel.encryption_enabled.Value() == false);
    QVERIFY(*retrieved_channel[0].message_channel.close_group_channel.Value() == true);
    QVERIFY(retrieved_channel[0].member_type == e8::MessageChannelMemberType::MCMT_ADMIN);
}

QTEST_APPLESS_MAIN(message_channel_test)

#include "tst_message_channel_test.moc"
