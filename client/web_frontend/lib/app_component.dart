import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/account.dart';
import 'package:demoweb_app/src/authorization_service.dart';
import 'package:demoweb_app/src/authorization_service_interface.dart';
import 'package:demoweb_app/src/fetch_realtime_message.dart';
import 'package:demoweb_app/src/message_channel_service.dart';
import 'package:demoweb_app/src/message_channel_service_interface.dart';
import 'package:demoweb_app/src/message_subscriber_service.dart';
import 'package:demoweb_app/src/message_subscriber_service_interface.dart';
import 'package:demoweb_app/src/navigation_component.dart';
import 'package:demoweb_app/src/route_paths.dart';
import 'package:demoweb_app/src/routes.dart';
import 'package:demoweb_app/src/socialnetwork_service.dart';
import 'package:demoweb_app/src/socialnetwork_service_interface.dart';
import 'package:demoweb_app/src/user_service.dart';
import 'package:demoweb_app/src/user_service_interface.dart';

@Component(
  selector: 'app',
  templateUrl: 'app_component.html',
  directives: [routerDirectives, NavigationComponent],
  styleUrls: ["app_component.css"],
  providers: [
    ClassProvider(UserServiceInterface, useClass: UserServiceImpl),
    ClassProvider(AuthorizationServiceInterface,
        useClass: AuthorizationServiceImpl),
    ClassProvider(SocialNetworkServiceInterface,
        useClass: SocialNetworkServiceImpl),
    ClassProvider(MessageChannelServiceInterface,
        useClass: MessageChannelServiceImpl),
    ClassProvider(MessageSubscriberServiceInterface,
        useClass: MessageSubscriberServiceImpl),
  ],
  exports: [RoutePaths, Routes],
)
class AppComponent {
  final UserServiceInterface _user_service;
  final AuthorizationServiceInterface _auth_service;
  final MessageSubscriberServiceInterface _message_subscriber_service;

  AppComponent(this._user_service, this._auth_service,
      this._message_subscriber_service) {
    signIn(null, _user_service, _auth_service);

    // fetchRealTimeMessages(1, _message_subscriber_service);
  }
}
