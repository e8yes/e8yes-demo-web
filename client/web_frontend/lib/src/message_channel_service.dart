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

import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pbgrpc.dart';

class MessageChannelServiceImpl extends MessageChannelServiceInterface {
  @override
  Future<CreateMessageChannelResponse> createMessageChannel(
      CreateMessageChannelRequest request, String signature) {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return MessageChannelServiceClient(channel)
          .createMessageChannel(request, options: call_opts);
    });
  }

  @override
  Future<AddUserToMessageChannelResponse> addUserToMessageChannel(
      AddUserToMessageChannelRequest request, String signature) {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return MessageChannelServiceClient(channel)
          .addUserToMessageChannel(request, options: call_opts);
    });
  }

  @override
  Future<SearchMessageChannelsResponse> searchMessageChannels(
      SearchMessageChannelsRequest request, String signature) {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return MessageChannelServiceClient(channel)
          .searchMessageChannels(request, options: call_opts);
    });
  }

  @override
  Future<GetMessageChannelMembersResponse> getMessageChannelMembers(
      GetMessageChannelMembersRequest request, String signature) {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return MessageChannelServiceClient(channel)
          .getMessageChannelMembers(request, options: call_opts);
    });
  }
}
