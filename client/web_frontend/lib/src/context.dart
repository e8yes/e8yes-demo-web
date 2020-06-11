import 'package:demoweb_app/src/connection_manager_interface.dart';
import 'package:demoweb_app/src/immediate_connection.dart';

Uri demowebServiceUri = Uri.parse("http://localhost:8000");

ConnectionManagerInterface demowebServiceConnections = 
  ImmediateConnection(demowebServiceUri);

const int kSecurityKeyLength = 128;
