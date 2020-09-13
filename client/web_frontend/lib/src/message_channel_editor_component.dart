import 'dart:async';

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/proto_dart/message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/pagination.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_message_channel.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/socialnetwork_service_interface.dart';
import 'package:demoweb_app/src/sync_search_result.dart';
import 'package:fixnum/fixnum.dart';

class MessageChannelEditorStructuralControl {
  List<UserPublicProfile> searchedContacts = List<UserPublicProfile>();
  bool onEditingMessageChannel = false;
  bool onLoadingSearchedContact = false;
}

class EditMessageChannelForm {
  String title = "";
  String description = "";
  List<UserPublicProfile> members = List<UserPublicProfile>();
}

@Component(
  selector: "message-channel-editor",
  templateUrl: "message_channel_editor_component.html",
  styleUrls: ["message_channel_editor_component.css"],
  directives: [
    coreDirectives,
    formDirectives,
  ],
)
class MessageChannelEditorComponent {
  @Input()
  MessageChannelOveriew messageChannel;

  StreamController<bool> editCompletionTriggerStreamController =
      StreamController<bool>();
  @Output()
  Stream<bool> get editCompletionTrigger =>
      editCompletionTriggerStreamController.stream;

  EditMessageChannelForm editMessageChannelForm = EditMessageChannelForm();
  MessageChannelEditorStructuralControl structuralControl =
      MessageChannelEditorStructuralControl();

  final MessageChannelServiceInterface _messageChannelService;
  final SocialNetworkServiceInterface _socialNetworkService;
  final SearchResultSync<SearchRelatedUserListResponse> _searchSync =
      SearchResultSync<SearchRelatedUserListResponse>();

  MessageChannelEditorComponent(
      this._socialNetworkService, this._messageChannelService);

  String dateToString(Int64 timestamp) {
    return DateTime.fromMillisecondsSinceEpoch(timestamp.toInt() * 1000)
        .toLocal()
        .toString();
  }

  void onKeyDownSearchContact(String searchInput) {
    SearchRelatedUserListRequest req = SearchRelatedUserListRequest();
    req.searchTerms = (NullableString()..value = searchInput);
    req.pagination = (Pagination()
      ..resultPerPage = 10
      ..pageNumber = 0);

    structuralControl.onLoadingSearchedContact = true;
    Future<SearchRelatedUserListResponse> currentSearchFuture =
        _socialNetworkService.searchRelatedUserList(
            req, credentialStorage.loadSignature());
    _searchSync.takeLatestFuture(currentSearchFuture,
        (SearchRelatedUserListResponse res) {
      structuralControl.searchedContacts = res.userProfiles;
      structuralControl.onLoadingSearchedContact = false;
    });
  }

  void onClickSearchContact(UserPublicProfile to_be_member) {
    structuralControl.searchedContacts.clear();
    editMessageChannelForm.members.add(to_be_member);
  }

  void onClickConfirmCreateMessageChannel() {
    String strippedTitle = editMessageChannelForm.title.replaceAll(" ", "");
    String strippedDescription =
        editMessageChannelForm.description.replaceAll(" ", "");

    CreateMessageChannelRequest request = CreateMessageChannelRequest();
    if (strippedTitle.isNotEmpty) {
      request.title = (NullableString()..value = editMessageChannelForm.title);
    }
    if (strippedDescription.isNotEmpty) {
      request.description =
          (NullableString()..value = editMessageChannelForm.description);
    }
    for (UserPublicProfile member in editMessageChannelForm.members) {
      request.memberIds.add(member.userId);
    }

    structuralControl.onEditingMessageChannel = true;
    _messageChannelService
        .createMessageChannel(request, credentialStorage.loadSignature())
        .then((CreateMessageChannelResponse res) {
      structuralControl.onEditingMessageChannel = false;
      editCompletionTriggerStreamController.add(true);
    });
  }

  void onClickCancelEditMessageChannel() {
    editCompletionTriggerStreamController.add(false);
  }
}
