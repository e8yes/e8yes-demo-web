import 'package:angular_router/angular_router.dart';
import 'package:demoweb_app/src/basic/context.dart';

class RoutePaths {
  static final account = RoutePath(path: "account/:$kIdPathVariable");
  static final contactList = RoutePath(path: "contactList");
  static final demoList = RoutePath(path: "demoList");
  static final wmChat = RoutePath(path: "wmChat/:$kIdPathVariable");
}
