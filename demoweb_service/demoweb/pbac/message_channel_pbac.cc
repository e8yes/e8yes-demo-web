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

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "demoweb_service/demoweb/pbac/message_channel_attributes.h"
#include "demoweb_service/demoweb/pbac/message_channel_member_attributes.h"
#include "demoweb_service/demoweb/pbac/message_channel_pbac.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/message_channel.pb.h"

namespace e8 {
namespace {

unsigned AdminCount(std::unordered_map<UserId, MessageChannelMemberAttributes> const &members) {
    unsigned num_admins = 0;
    for (auto const &[_, member] : members) {
        num_admins += member.member_type == MCMT_ADMIN;
    }
    return num_admins;
}

} // namespace

MessageChannelPbacInterface::MessageChannelPbacInterface() {}

MessageChannelPbacInterface::~MessageChannelPbacInterface() {}

MessageChannelPbacImpl::MessageChannelPbacImpl(ConnectionReservoirInterface *conns)
    : conns_(conns) {}

MessageChannelPbacImpl::~MessageChannelPbacImpl() {}

bool MessageChannelPbacImpl::AllowUpdateChannelMetadata(UserId const operator_user_id,
                                                        MessageChannelId const target_channel_id) {
    std::optional<MessageChannelAttributes> channel_attrs =
        ExtractMessageChannelAttributes(target_channel_id, conns_);
    if (!channel_attrs.has_value()) {
        return false;
    }

    std::optional<MessageChannelMemberAttributes> operator_attrs =
        ExtractMessageChannelMemberAttributes(target_channel_id, operator_user_id, conns_);
    if (!operator_attrs.has_value()) {
        return false;
    }

    if (*channel_attrs->close_group_channel.Value()) {
        return true;
    }

    return operator_attrs->member_type == MCMT_ADMIN;
}

bool MessageChannelPbacImpl::AllowDeleteChannel(UserId const operator_user_id,
                                                MessageChannelId const target_channel_id) {
    std::optional<MessageChannelMemberAttributes> operator_attrs =
        ExtractMessageChannelMemberAttributes(target_channel_id, operator_user_id, conns_);
    if (!operator_attrs.has_value()) {
        return false;
    }

    return operator_attrs->member_type == MCMT_ADMIN;
}

bool MessageChannelPbacImpl::AllowUpdateChannelMembership(UserId const operator_user_id,
                                                          MessageChannelId const target_channel_id,
                                                          UserId const user_to_be_updated,
                                                          MessageChannelMemberType member_type) {
    std::optional<MessageChannelAttributes> channel_attrs =
        ExtractMessageChannelAttributes(target_channel_id, conns_);
    if (!channel_attrs.has_value()) {
        return false;
    }

    std::unordered_map<UserId, MessageChannelMemberAttributes> member_attrs =
        ExtractMessageChannelMemberAttributes(target_channel_id, conns_);
    auto operator_attr_it = member_attrs.find(operator_user_id);
    if (operator_attr_it == member_attrs.end()) {
        // Operator must as least be a member.
        return false;
    }

    switch (member_type) {
    case MCMT_ADMIN: {
        // Adding a new admin or promotion.
        return operator_attr_it->second.member_type == MCMT_ADMIN;
    }

    case MCMT_MEMBER: {
        auto to_be_updatec_attr_it = member_attrs.find(user_to_be_updated);

        if (to_be_updatec_attr_it != member_attrs.end()) {
            // Demotion.
            if (operator_attr_it->second.member_type != MCMT_ADMIN) {
                return false;
            }

            if (to_be_updatec_attr_it->second.member_type == MCMT_ADMIN &&
                AdminCount(member_attrs) == 1) {
                // Can't demote the last admin in the message channel.
                return false;
            }

            return true;
        }

        return *channel_attrs->close_group_channel.Value() ||
               operator_attr_it->second.member_type == MCMT_ADMIN;
    }

    case MCMT_INVALID:
    default:
        return false;
    }
}

bool MessageChannelPbacImpl::AllowDeleteMemberFromChannel(UserId const operator_user_id,
                                                          MessageChannelId const target_channel_id,
                                                          UserId const user_to_be_removed) {
    std::unordered_map<UserId, MessageChannelMemberAttributes> member_attrs =
        ExtractMessageChannelMemberAttributes(target_channel_id, conns_);
    auto operator_attr_it = member_attrs.find(operator_user_id);
    if (operator_attr_it == member_attrs.end()) {
        return false;
    }

    auto to_be_removed_attr_it = member_attrs.find(user_to_be_removed);
    if (to_be_removed_attr_it == member_attrs.end()) {
        return false;
    }

    if (to_be_removed_attr_it->second.member_type == MCMT_ADMIN && AdminCount(member_attrs) == 1) {
        // Can't remove the only admin.
        return false;
    }

    return operator_user_id == user_to_be_removed ||
           operator_attr_it->second.member_type == MCMT_ADMIN;
}

bool MessageChannelPbacImpl::AllowCreateChatMessageGroup(UserId const operator_user_id,
                                                         MessageChannelId const target_channel_id) {
    return ExtractMessageChannelMemberAttributes(target_channel_id, operator_user_id, conns_)
        .has_value();
}

bool MessageChannelPbacImpl::AllowSendChatMessage(UserId const operator_user_id,
                                                  MessageChannelId const target_channel_id) {
    return this->AllowCreateChatMessageGroup(operator_user_id, target_channel_id);
}

} // namespace e8
