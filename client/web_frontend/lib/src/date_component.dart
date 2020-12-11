import 'package:angular/angular.dart';
import 'package:demoweb_app/src/basic/date_string.dart';
import 'package:fixnum/fixnum.dart';

@Component(
    selector: "date-hour",
    templateUrl: "date_component.html",
    styleUrls: ["date_component.css"],
    directives: [coreDirectives],
    exports: [extractDateString, extractHourString])
class DateComponent {
  @Input()
  Int64 timestampMicros;
}
