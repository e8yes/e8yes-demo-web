import 'dart:async';

import 'package:angular/angular.dart';
import 'package:angular/core.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/footer_component.dart';
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

class MessageChannelStructuralControl {
  List<MessageChannelOveriew> messageChannelOverviews =
      List<MessageChannelOveriew>();
  List<UserPublicProfile> searchedContacts = List<UserPublicProfile>();
  bool onLoadingMessageChannels = false;
  bool showAddMessageChannelPopup = false;
  bool onCreatingMessageChannel = false;
  bool onLoadingSearchedContact = false;
}

class CreateMessageChannelForm {
  String title = "";
  String description = "";
  List<UserPublicProfile> members = List<UserPublicProfile>();
}

@Component(
    selector: "message-channel-manager",
    templateUrl: "message_channel_manager_component.html",
    styleUrls: ["message_channel_manager_component.css"],
    directives: [coreDirectives, formDirectives, FooterComponent],
    exports: [MessageChannelStructuralControl, CreateMessageChannelForm])
class MessageChannelManagerComponent implements OnInit {
  MessageChannelStructuralControl structuralControl =
      MessageChannelStructuralControl();
  CreateMessageChannelForm createMessageChannelForm =
      CreateMessageChannelForm();

  @Input()
  Int64 targetMemberId;

  StreamController<MessageChannelOveriew>
      currentMessageChannelStreamController =
      StreamController<MessageChannelOveriew>();
  @Output()
  Stream<MessageChannelOveriew> get currentMessageChannel =>
      currentMessageChannelStreamController.stream;

  final MessageChannelServiceInterface _messageChannelService;
  final SocialNetworkServiceInterface _socialNetworkService;
  final SearchResultSync<SearchRelatedUserListResponse> _searchSync =
      SearchResultSync<SearchRelatedUserListResponse>();

  static const int _kActiveUsersFetchLimit = 5;

  MessageChannelManagerComponent(
      this._messageChannelService, this._socialNetworkService) {}

  @override
  void ngOnInit() {
    this._fetchMessageChannelList();
  }

  void onKeyDownSearchMessageChannel(String searchTerms) {}

  void onClickCreateMessageChannel() {
    structuralControl.showAddMessageChannelPopup = true;
  }

  void onClickCancelCreateMessageChannel() {
    structuralControl.showAddMessageChannelPopup = false;
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
    createMessageChannelForm.members.add(to_be_member);
  }

  void onClickConfirmCreateMessageChannel() {
    createMessageChannelForm.title.replaceAll(" ", "");
    createMessageChannelForm.description.replaceAll(" ", "");

    CreateMessageChannelRequest request = CreateMessageChannelRequest();
    if (createMessageChannelForm.title.isNotEmpty) {
      request.title =
          (NullableString()..value = createMessageChannelForm.title);
    }
    if (createMessageChannelForm.description.isNotEmpty) {
      request.description =
          (NullableString()..value = createMessageChannelForm.description);
    }
    for (UserPublicProfile member in createMessageChannelForm.members) {
      request.memberIds.add(member.userId);
    }

    structuralControl.onCreatingMessageChannel = true;
    _messageChannelService
        .createMessageChannel(request, credentialStorage.loadSignature())
        .then((CreateMessageChannelResponse res) {
      structuralControl.onCreatingMessageChannel = false;
      structuralControl.showAddMessageChannelPopup = false;

      _fetchMessageChannelList();
    });
  }

  void onClickMessageChannel(MessageChannelOveriew channel) {
    currentMessageChannelStreamController.add(channel);
  }

  String dateToString(Int64 timestamp) {
    return DateTime.fromMillisecondsSinceEpoch(timestamp.toInt() * 1000)
        .toLocal()
        .toString();
  }

  void _fetchMessageChannelList() {
    GetJoinedInMessageChannelsRequest request =
        GetJoinedInMessageChannelsRequest();
    if (targetMemberId != null) {
      request.withMemberIds.add(targetMemberId);
    }
    request.activeMemberFetchLimit = _kActiveUsersFetchLimit;

    structuralControl.onLoadingMessageChannels = true;
    _messageChannelService
        .getJoinedInMessageChannels(request, credentialStorage.loadSignature())
        .then((GetJoinedInMessageChannelsResponse res) {
      structuralControl.messageChannelOverviews = res.channels;
      structuralControl.onLoadingMessageChannels = false;
    });
  }
}
