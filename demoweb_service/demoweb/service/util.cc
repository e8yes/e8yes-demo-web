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

#include <grpcpp/grpcpp.h>

#include "demoweb_service/demoweb/constant/context_key.h"
#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/module_identity/user_identity.h"
#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "demoweb_service/demoweb/service/util.h"

namespace e8 {

grpc::Status ExtractIdentityFromContext(grpc::ServerContext const &context, Identity *identity) {
    auto context_it = context.client_metadata().find(kAuthorizationKey);
    if (context_it == context.client_metadata().end()) {
        return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Expect a signed identity.");
    }

    std::optional<Identity> identity_optional =
        ValidateSignedIdentity(std::string(context_it->second.data(), context_it->second.size()),
                               CurrentEnvironment()->KeyGen());
    if (!identity_optional.has_value()) {
        return grpc::Status(grpc::StatusCode::UNAUTHENTICATED,
                            "Failed to decode the signed identity.");
    }

    *identity = identity_optional.value();

    return grpc::Status::OK;
}

} // namespace e8
