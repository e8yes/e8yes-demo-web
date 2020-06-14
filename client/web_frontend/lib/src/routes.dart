import 'package:angular_router/angular_router.dart';

import 'package:demoweb_app/src/account_component.template.dart'
    as account_template;
import 'package:demoweb_app/src/route_paths.dart';

class Routes {
  static final account = RouteDefinition(
    routePath: RoutePaths.account,
    component: account_template.AccountComponentNgFactory,
  );

  static final all = <RouteDefinition>[
    account,
  ];
}
