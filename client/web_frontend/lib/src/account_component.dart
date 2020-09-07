import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/context.dart';
import 'package:demoweb_app/src/footer_component.dart';
import 'package:demoweb_app/src/profile_component.dart';
import 'package:demoweb_app/src/proto_dart/service_socialnetwork.pbgrpc.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/proto_dart/user_relation.pb.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:demoweb_app/src/socialnetwork_service_interface.dart';
import 'package:demoweb_app/src/user_service_interface.dart';
import 'package:fixnum/fixnum.dart';

@Component(
  selector: "account",
  templateUrl: "account_component.html",
  directives: [coreDirectives, ProfileComponent, FooterComponent],
)
class AccountComponent implements OnActivate {
  UserPublicProfile profile = UserPublicProfile();
  bool onUpdateRelation = false;

  final UserServiceInterface _user_service;
  final SocialNetworkServiceInterface _social_network_service;
  Int64 _accountUserId;

  AccountComponent(this._user_service, this._social_network_service);

  void _fetchAccountProfile(Int64 accountUserId) {
    GetPublicProfileRequest req = GetPublicProfileRequest();
    req.userId = accountUserId;

    _user_service
        .getPublicProfile(req, credentialStorage.loadSignature())
        .then((GetPublicProfileResponse res) {
      profile = res.profile;
    });
  }

  @override
  void onActivate(_, RouterState current) async {
    final userId = getIdPathVariable(current.parameters);
    if (userId != null) {
      _accountUserId = userId;
    } else {
      _accountUserId = identityStorage.loadUserId();
    }
    if (_accountUserId != null) {
      _fetchAccountProfile(_accountUserId);
    }
  }

  void onClickAddContact() {
    SendInvitationRequest req = SendInvitationRequest();
    req.inviteeUserId = _accountUserId;

    onUpdateRelation = true;

    _social_network_service
        .sendInvitation(req, credentialStorage.loadSignature())
        .then((SendInvitationResponse res) {
      _fetchAccountProfile(_accountUserId);

      onUpdateRelation = false;
    });
  }

  void onClickConfirmContact() {
    ProcessInvitationRequest req = ProcessInvitationRequest()
      ..inviterUserId = _accountUserId
      ..accept = true;

    onUpdateRelation = true;

    _social_network_service
        .processInvitation(req, credentialStorage.loadSignature())
        .then((ProcessInvitationResponse res) {
      this._fetchAccountProfile(_accountUserId);

      onUpdateRelation = false;
    });
  }

  void onClickRejectContact() {
    ProcessInvitationRequest req = ProcessInvitationRequest()
      ..inviterUserId = _accountUserId
      ..accept = false;

    onUpdateRelation = true;

    _social_network_service
        .processInvitation(req, credentialStorage.loadSignature())
        .then((ProcessInvitationResponse res) {
      this._fetchAccountProfile(_accountUserId);

      onUpdateRelation = false;
    });
  }

  void onClickDeleteContact() {
    DeleteContactRequest req = DeleteContactRequest()
      ..deletedContactUserId = _accountUserId;

    onUpdateRelation = true;

    _social_network_service
        .deleteContact(req, credentialStorage.loadSignature())
        .then((DeleteContactResponse res) {
      this._fetchAccountProfile(_accountUserId);

      onUpdateRelation = false;
    });
  }

  bool owner() {
    return _accountUserId == identityStorage.loadUserId();
  }

  bool addContactMode() {
    return !owner() && profile.relations.isEmpty;
  }

  bool invitationPendingMode() {
    return !owner() &&
        profile.relations.any((UserRelationRecord relation) =>
            relation.relation == UserRelation.URL_INVITATION_SENT);
  }

  bool invitaitonRecievedMode() {
    return !owner() &&
        profile.relations.any((UserRelationRecord relation) =>
            relation.relation == UserRelation.URL_INVITATION_RECEIVED);
  }

  bool contactMode() {
    return !owner() &&
        profile.relations.any((UserRelationRecord relation) =>
            relation.relation == UserRelation.URL_CONTACT);
  }
}
