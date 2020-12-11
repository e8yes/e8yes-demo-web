import 'package:angular/angular.dart';
import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/account/account.dart';
import 'package:demoweb_app/src/service/authorization_service.dart';
import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:demoweb_app/src/basic/fetch_realtime_message.dart';
import 'package:demoweb_app/src/service/chat_message_service.dart';
import 'package:demoweb_app/src/service/chat_message_service_interface.dart';
import 'package:demoweb_app/src/service/message_channel_service.dart';
import 'package:demoweb_app/src/service/message_channel_service_interface.dart';
import 'package:demoweb_app/src/service/message_subscriber_service.dart';
import 'package:demoweb_app/src/service/message_subscriber_service_interface.dart';
import 'package:demoweb_app/src/navigation_component.dart';
import 'package:demoweb_app/src/routes/route_paths.dart';
import 'package:demoweb_app/src/routes/routes.dart';
import 'package:demoweb_app/src/service/socialnetwork_service.dart';
import 'package:demoweb_app/src/service/socialnetwork_service_interface.dart';
import 'package:demoweb_app/src/service/user_service.dart';
import 'package:demoweb_app/src/service/user_service_interface.dart';

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
    ClassProvider(ChatMessageServiceInterface,
        useClass: ChatMessageServiceImpl),
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

    fetchRealTimeMessages(1, _message_subscriber_service);
  }
}
