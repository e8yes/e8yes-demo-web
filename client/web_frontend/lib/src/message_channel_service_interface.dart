import "dart:async";

import "proto_dart/service_message_channel.pb.dart";

abstract class MessageChannelServiceInterface {
  Future<CreateMessageChannelResponse> createMessageChannel(
      CreateMessageChannelRequest request);

  Future<AddUserToMessageChannelResponse> addUserToMessageChannel(
      AddUserToMessageChannelRequest request);

  Future<GetJoinedInMessageChannelsResponse> getJoinedInMessageChannels(
      GetJoinedInMessageChannelsRequest request);

  Future<GetMessageChannelMembersResponse> getMessageChannelMembers(
      GetMessageChannelMembersRequest request);
}
