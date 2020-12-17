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

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/component/chat/chat_message_group_component.dart';
import 'package:demoweb_app/src/component/chat/chat_message_group_editor_component.dart';
import 'package:demoweb_app/src/component/message_channel/message_channel_overview_component.dart';
import 'package:demoweb_app/src/proto_dart/chat_message.pb.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/pagination.pbserver.dart';
import 'package:demoweb_app/src/proto_dart/service_chat_message.pbgrpc.dart';
import 'package:demoweb_app/src/service/chat_message_service_interface.dart';

@Component(
  selector: "chat-room",
  templateUrl: "chat_room_component.html",
  styleUrls: ["chat_room_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
    ChatMessageGroupComponent,
    ChatMessageGroupEditorComponent,
    MessageChannelOverviewComponent,
  ],
)
class ChatRoomComponent implements OnInit {
  @Input()
  MessageChannelOverview messageChannel;

  // Presentation data.
  bool chatMessageThreadsLoading = true;
  List<ChatMessageThread> chatMessageThreads = List<ChatMessageThread>();

  // Dependencies.
  ChatMessageServiceInterface _chatMessageService;

  ChatRoomComponent(this._chatMessageService);

  @override
  void ngOnInit() {
    this._fetchChatMessageThreads();
  }

  void _fetchChatMessageThreads() {
    GetChatMessageThreadsRequest request = GetChatMessageThreadsRequest();
    request.channelId = messageChannel.channel.channelId;
    request.limitPerThread = 5;
    request.pagination = Pagination()
      ..pageNumber = 0
      ..resultPerPage = 5;

    chatMessageThreadsLoading = true;
    _chatMessageService
        .getChatMessageThreads(request, credentialStorage.loadSignature())
        .then((GetChatMessageThreadsResponse res) {
      chatMessageThreads = res.threads;
      chatMessageThreadsLoading = false;
    });
  }
}
