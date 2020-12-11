/**
 * e8yes demo web.
 *
 * <p>Copyright (C) 2020 Chifeng Wen {daviesx66@gmail.com}
 *
 * <p>This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * <p>This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * <p>You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
 */

import 'package:demoweb_app/src/service/authorization_service_interface.dart';
import 'package:demoweb_app/src/basic/context.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pb.dart';
import 'package:demoweb_app/src/proto_dart/service_user.pbgrpc.dart';

class AuthorizationServiceImpl implements AuthorizationServiceInterface {
  @override
  Future<AuthorizationResponse> authorize(AuthorizationRequest request) async {
    return demowebServiceRequester.MakeRequest(request, null,
        (request, call_opts, channel) {
      return UserServiceClient(channel).authorize(request);
    });
  }
}
