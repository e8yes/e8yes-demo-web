import "dart:async";

import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/proto_dart/pagination.pbserver.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/sync_search_result.dart';
import 'package:demoweb_app/src/user_service_interface.dart';
import 'package:fixnum/fixnum.dart';


@Component(
  selector: "contact-list",
  templateUrl: "contact_list_component.html",
  styleUrls: ["contact_list_component.css"],
  directives: [coreDirectives, formDirectives],
)
class ContactListComponent {
  List<UserPublicProfile> profiles = List<UserPublicProfile>();
  UserPublicProfile selectedUser;

  static const int _kResultPerPage = 20;
  final UserServiceInterface _user_service;

  SearchResultSync<SearchUserResponse> _searchSync =
      SearchResultSync<SearchUserResponse>();

  ContactListComponent(this._user_service);

  void onKeyDownSearchContact(String searchInput) {
    SearchUserRequest req = SearchUserRequest();
    req.alias = (NullableString()..value = searchInput);
    try {
      Int64 userIdSearchPrefix = Int64.parseInt(searchInput);
      req.userId = (NullableInt64()..value = userIdSearchPrefix);
    } catch (err) {}
    Pagination pagination = Pagination();
    pagination.resultPerPage = _kResultPerPage;
    pagination.pageNumber = 0;
    req.pagination = pagination;

    Future<SearchUserResponse> currentSearchFuture = _user_service.search(req);
    _searchSync.takeLatestFuture(currentSearchFuture, (SearchUserResponse res) {
      profiles = res.userProfiles;
    });
  }

  void onSelectSearchedContact(UserPublicProfile userProfile) {
    print(userProfile.userId);
  }
}
