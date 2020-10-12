#ifndef MESSAGE_CHANNEL_MEMBER_ATTRIBUTES_H
#define MESSAGE_CHANNEL_MEMBER_ATTRIBUTES_H

#include <optional>
#include <unordered_map>

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/message_channel.pb.h"

namespace e8 {

struct MessageChannelMemberAttributes {
    MessageChannelMemberType member_type;
};

/**
 * @brief ExtractMessageChannelMemberAttributes Extract a message channel member's attributes needed
 * for access control.
 */
std::unordered_map<UserId, MessageChannelMemberAttributes>
ExtractMessageChannelMemberAttributes(MessageChannelId const channel_id,
                                      ConnectionReservoirInterface *conns);

/**
 * @brief ExtractMessageChannelMemberAttribute Similar to the above function, it extracts attributes
 * of all the channel members at once instead.
 */
std::optional<MessageChannelMemberAttributes>
ExtractMessageChannelMemberAttributes(MessageChannelId const channel_id, UserId const user_id,
                                      ConnectionReservoirInterface *conns);

} // namespace e8

#endif // MESSAGE_CHANNEL_MEMBER_ATTRIBUTES_H
