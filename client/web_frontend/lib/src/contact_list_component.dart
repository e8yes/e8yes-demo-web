import "dart:async";

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/proto_dart/pagination.pbserver.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_relation.pb.dart';
import 'package:demoweb_app/src/route_paths.dart';
import 'package:demoweb_app/src/socialnetwork_service_interface.dart';
import 'package:demoweb_app/src/sync_search_result.dart';
import 'package:demoweb_app/src/user_service_interface.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "contact-list",
  templateUrl: "contact_list_component.html",
  styleUrls: ["contact_list_component.css"],
  directives: [coreDirectives, formDirectives],
)
class ContactListComponent implements OnActivate {
  List<UserPublicProfile> searchedProfiles = List<UserPublicProfile>();
  List<UserPublicProfile> inviterProfiles = List<UserPublicProfile>();
  List<UserPublicProfile> contactProfiles = List<UserPublicProfile>();

  static const int _kResultPerPage = 20;
  final UserServiceInterface _user_service;
  final SocialNetworkServiceInterface _social_network_service;
  final Router _router;

  SearchResultSync<SearchUserResponse> _searchSync =
      SearchResultSync<SearchUserResponse>();

  ContactListComponent(
      this._user_service, this._social_network_service, this._router);

  @override
  void onActivate(_, RouterState current) async {
    _social_network_service
        .getInvitationList(
            GetInvitationListRequest(), credentialStorage.loadSignature())
        .then((GetInvitationListResponse res) {
      inviterProfiles = res.userProfiles;
    });
  }

  void onKeyDownSearchContact(String searchInput) {
    SearchUserRequest req = SearchUserRequest();
    if (searchInput.isNotEmpty) {
      req.alias = (NullableString()..value = searchInput);
      try {
        Int64 userIdSearchPrefix = Int64.parseInt(searchInput);
        req.userId = (NullableInt64()..value = userIdSearchPrefix);
      } catch (err) {}
    }
    Pagination pagination = Pagination();
    pagination.resultPerPage = _kResultPerPage;
    pagination.pageNumber = 0;
    req.pagination = pagination;

    String viewerSignature = credentialStorage.loadSignature();
    Future<SearchUserResponse> currentSearchFuture =
        _user_service.search(req, viewerSignature);
    _searchSync.takeLatestFuture(currentSearchFuture, (SearchUserResponse res) {
      searchedProfiles = res.userProfiles;
    });
  }

  String _accountDetailsUrl(Int64 userId) {
    return RoutePaths.account.toUrl(parameters: {kIdPathVariable: "$userId"});
  }

  void onSelectUserDetails(UserPublicProfile userProfile) {
    _router.navigate(_accountDetailsUrl(userProfile.userId));
  }

  bool unrelated(List<UserRelation> relations) {
    return relations.isEmpty;
  }

  bool invitationPending(List<UserRelation> relations) {
    return relations.contains(UserRelation.URL_INVITATION_SENT);
  }

  bool invitationReceived(List<UserRelation> relations) {
    return relations.contains(UserRelation.URL_INVITATION_RECEIVED);
  }

  bool contact(List<UserRelation> relations) {
    return relations.contains(UserRelation.URL_CONTACT);
  }

  bool blocked(List<UserRelation> relations) {
    return relations.contains(UserRelation.URL_BLOCKED);
  }

  bool blocking(List<UserRelation> relations) {
    return relations.contains(UserRelation.URL_BLOCKING);
  }
}
