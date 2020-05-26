/**
 * e8yes demo web server.
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

#ifndef UTIL_H
#define UTIL_H

#include <grpcpp/grpcpp.h>

#include "demoweb_service/demoweb/proto_cc/identity.pb.h"

namespace e8 {

/**
 * @brief ExtractIdentityFromContext Utility function to validate and decode user identity from the
 * server context.
 *
 * @return The status for the extraction. If there is no error, the extracted identity will be
 * written to the identity argument. Otherwise, the value in the identity argument is undefined.
 */
grpc::Status ExtractIdentityFromContext(grpc::ServerContext const &context, Identity *identity);

} // namespace e8

#endif // UTIL_H
