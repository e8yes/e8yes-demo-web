import 'package:angular/angular.dart';
import 'package:demoweb_app/src/proto_dart/user_profile.pb.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

@Component(
  selector: 'profile',
  templateUrl: 'profile_component.html',
  directives: [coreDirectives],
)
class ProfileComponent {
  @Input() UserPublicProfile profile;

  final UserServiceInterface user_service_;

  ProfileComponent(this.user_service_);
}
