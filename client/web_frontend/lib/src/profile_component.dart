import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/basic/date_string.dart';
import 'package:demoweb_app/src/date_component.dart';
import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/service/user_service_interface.dart';

@Component(
  selector: 'profile',
  templateUrl: 'profile_component.html',
  styleUrls: ["profile_component.css"],
  exports: [extractDateString, extractHourString],
  directives: [coreDirectives, formDirectives, DateComponent],
)
class ProfileComponent {
  @Input()
  UserPublicProfile profile;
  @Input()
  bool editable;
  bool editMode = false;
  bool onUpdate = false;
  String newAlias;
  final UserServiceInterface _user_service;

  ProfileComponent(this._user_service);

  bool hasAlias(UserPublicProfile profile) {
    return profile.hasAlias() && profile.alias.value.isNotEmpty;
  }

  void onClickEdit() {
    editMode = true;
  }

  void onClickConfirm() {
    UpdatePublicProfileRequest req = UpdatePublicProfileRequest();
    if (newAlias != null) {
      profile.alias = (NullableString()..value = newAlias);
    }
    req.alias = profile.alias;

    onUpdate = true;
    _user_service
        .updatePublicProfile(req, credentialStorage.loadSignature())
        .then((UpdatePublicProfileResponse res) {
      onUpdate = false;
      editMode = false;
    });
  }

  void onClickCancel() {
    editMode = false;
  }
}
