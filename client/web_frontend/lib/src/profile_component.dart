import 'package:angular/angular.dart';
import 'package:angular_forms/angular_forms.dart';
import 'package:demoweb_app/src/context.dart';

import 'package:demoweb_app/src/proto_dart/nullable_primitives.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

@Component(
  selector: 'profile',
  templateUrl: 'profile_component.html',
  styleUrls: ["profile_component.css"],
  directives: [coreDirectives, formDirectives],
)
class ProfileComponent {
  @Input()
  UserPublicProfile profile;

  @Input()
  bool editable;

  bool editMode = false;

  final UserServiceInterface _user_service;

  ProfileComponent(this._user_service);

  void onClickEdit() {
    editMode = true;
  }

  void onClickConfirm() {
    UpdatePublicProfileRequest req = UpdatePublicProfileRequest();
    req.alias = (NullableString()..value = profile.alias);
    
    String signature = credentialStorage.loadSignature();

    _user_service
        .updatePublicProfile(req, signature)
        .then((UpdatePublicProfileResponse res) {
      editMode = false;
    });
  }

  void onClickCancel() {
    editMode = false;
  }
}
