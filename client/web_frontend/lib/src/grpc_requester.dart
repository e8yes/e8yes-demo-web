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

import 'package:demoweb_app/src/connection_manager_interface.dart';
import 'package:grpc/grpc_web.dart';

class GrpcRequester {
  final ConnectionManagerInterface _conn_mgr;

  GrpcRequester(this._conn_mgr);

  Response MakeRequest<Request, Response>(
      Request request, String signature, Function requestCall) {
    GrpcWebClientChannel channel = _conn_mgr.take();
    Response res = requestCall(
        request,
        CallOptions(metadata: {'a': signature}, timeout: Duration(seconds: 60)),
        channel);
    _conn_mgr.put(channel);
    return res;
  }
}
