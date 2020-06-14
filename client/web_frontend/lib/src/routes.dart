import 'package:angular_router/angular_router.dart';

import 'package:demoweb_app/src/account_component.template.dart'
    as account_template;
import 'package:demoweb_app/src/contact_list_component.template.dart'
    as contact_list_template;
import 'package:demoweb_app/src/demo_list_component.template.dart'
    as demo_list_template;
import 'package:demoweb_app/src/route_paths.dart';

class Routes {
  static final account = RouteDefinition(
    routePath: RoutePaths.account,
    component: account_template.AccountComponentNgFactory,
  );

  static final contactList = RouteDefinition(
    routePath: RoutePaths.contactList,
    component: contact_list_template.ContactListComponentNgFactory,
  );

  static final demoList = RouteDefinition(
    routePath: RoutePaths.demoList,
    component: demo_list_template.DemoListComponentNgFactory
  );

  static final all = <RouteDefinition>[
    account,
    contactList,
    demoList
  ];
}
