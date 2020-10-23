#ifndef MESSAGECHANNELPBAC_H
#define MESSAGECHANNELPBAC_H

#include "demoweb_service/demoweb/common_entity/message_channel_entity.h"
#include "demoweb_service/demoweb/common_entity/user_entity.h"
#include "postgres/query_runner/connection/connection_reservoir_interface.h"
#include "proto_cc/message_channel.pb.h"

namespace e8 {

/**
 * @brief The MessageChannelPbacInterface class Defines access control policies surrounding the
 * message channel resource.
 */
class MessageChannelPbacInterface {
  public:
    MessageChannelPbacInterface();
    virtual ~MessageChannelPbacInterface();

    /**
     * @brief AllowUpdateChannelMetadata Check if the operator has the permission to change the
     * metadata of the specified message channel.
     *
     * @param operator_user_id ID of the person who is requesting the update.
     * @param target_channel_id ID of the message channel to be updated.
     * @return Whether the operator has the right.
     */
    virtual bool AllowUpdateChannelMetadata(UserId const operator_user_id,
                                            MessageChannelId const target_channel_id) = 0;

    /**
     * @brief AllowDeleteChannel Check if the operator has the permission to delete the specified
     * message channel.
     *
     * @param operator_user_id ID of the person who is requesting the deletion.
     * @param target_channel_id ID of the message channel to be deleted.
     * @return Whether the operator has the right.
     */
    virtual bool AllowDeleteChannel(UserId const operator_user_id,
                                    MessageChannelId const target_channel_id) = 0;

    /**
     * @brief AllowUpdateChannelMembership Check if the operator has the permission to add or change
     * the membership to the specified message channel.
     *
     * @param operator_user_id ID of the person who is requesting the change to the membership.
     * @param target_channel_id ID of the message channel to be changed membership from.
     * @param user_to_be_updated ID of the person to be update the channel membership of.
     * @param member_type The new member type the operator is requesting to update to.
     * @return Whether the operator has the right.
     */
    virtual bool AllowUpdateChannelMembership(UserId const operator_user_id,
                                              MessageChannelId const target_channel_id,
                                              UserId const user_to_be_updated,
                                              MessageChannelMemberType member_type) = 0;

    /**
     * @brief AllowDeleteMemberFromChannel Check if the operator has the permission to remove the
     * specified member from the message channel.
     *
     * @param operator_user_id ID of the person who is requesting the member's removal.
     * @param target_channel_id ID of the message channel to be updated with removal.
     * @param user_to_be_removed ID of the channel member to be removed.
     * @return Whether the operator has the right.
     */
    virtual bool AllowDeleteMemberFromChannel(UserId const operator_user_id,
                                              MessageChannelId const target_channel_id,
                                              UserId const user_to_be_removed) = 0;
};

/**
 * @brief The MessageChannelPbacImpl class Default implementation of the access control policy.
 */
class MessageChannelPbacImpl : public MessageChannelPbacInterface {
  public:
    MessageChannelPbacImpl(ConnectionReservoirInterface *conns);
    ~MessageChannelPbacImpl() override;

    bool AllowUpdateChannelMetadata(UserId const operator_user_id,
                                    MessageChannelId const target_channel_id) override;

    bool AllowDeleteChannel(UserId const operator_user_id,
                            MessageChannelId const target_channel_id) override;

    bool AllowUpdateChannelMembership(UserId const operator_user_id,
                                      MessageChannelId const target_channel_id,
                                      UserId const user_to_be_updated,
                                      MessageChannelMemberType member_type) override;

    bool AllowDeleteMemberFromChannel(UserId const operator_user_id,
                                      MessageChannelId const target_channel_id,
                                      UserId const user_to_be_removed) override;

  private:
    ConnectionReservoirInterface *conns_;
};

} // namespace e8

#endif // MESSAGECHANNELPBAC_H
