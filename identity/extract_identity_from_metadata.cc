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
#include <optional>
#include <string>

#include "identity/extract_identity_from_metadata.h"
#include "identity/trustable_identity.h"
#include "keygen/key_generator_interface.h"
#include "proto_cc/identity.pb.h"

namespace e8 {

std::optional<Identity> ExtractIdentityFromContext(grpc::ServerContext const &context,
                                                   std::string const &auth_key,
                                                   KeyGeneratorInterface *key_gen,
                                                   grpc::Status *status) {
    auto context_it = context.client_metadata().find(auth_key);
    if (context_it == context.client_metadata().end()) {
        if (status != nullptr) {
            *status = grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Expect a signed identity.");
        }
        return std::nullopt;
    }

    std::optional<Identity> identity_optional = ValidateSignedIdentity(
        std::string(context_it->second.data(), context_it->second.size()), key_gen);
    if (!identity_optional.has_value()) {
        if (status != nullptr) {
            *status = grpc::Status(grpc::StatusCode::UNAUTHENTICATED,
                                   "Failed to decode the signed identity.");
        }
        return std::nullopt;
    }

    if (status != nullptr) {
        *status = grpc::Status::OK;
    }
    return identity_optional;
}

} // namespace e8
