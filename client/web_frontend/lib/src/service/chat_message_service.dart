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

import "dart:async";

import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/service/chat_message_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/service_chat_message.pbgrpc.dart';

class ChatMessageServiceImpl extends ChatMessageServiceInterface {
  @override
  Future<CreateChatMessageThreadRequest> createChatMessageThread(
      CreateChatMessageThreadResponse request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return ChatMessageServiceClient(channel)
          .createChatMessageThread(request, options: call_opts);
    });
  }

  @override
  Future<GetChatMessageThreadsResponse> getChatMessageThreads(
      GetChatMessageThreadsRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return ChatMessageServiceClient(channel)
          .getChatMessageThreads(request, options: call_opts);
    });
  }

  @override
  Future<GetChatMessagesResponse> getChatMessages(
      GetChatMessagesRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return ChatMessageServiceClient(channel)
          .getChatMessages(request, options: call_opts);
    });
  }

  @override
  Future<SendChatMessageResponse> sendChatMessage(
      SendChatMessageRequest request, String signature) async {
    return demowebServiceRequester.MakeRequest(request, signature,
        (request, call_opts, channel) {
      return ChatMessageServiceClient(channel)
          .sendChatMessage(request, options: call_opts);
    });
  }
}
