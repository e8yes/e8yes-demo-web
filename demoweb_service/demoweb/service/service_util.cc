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

#include "demoweb_service/demoweb/environment/environment_context_interface.h"
#include "demoweb_service/demoweb/service/service_util.h"
#include "identity/auth_key.h"
#include "identity/extract_identity_from_metadata.h"
#include "proto_cc/identity.pb.h"
#include "proto_cc/pagination.pb.h"

namespace e8 {

std::optional<Identity> ExtractIdentityFromContext(grpc::ServerContext const &context,
                                                   grpc::Status *status) {
    return ExtractIdentityFromContext(context, kDemoWebUserAuthorizationKey,
                                      CurrentEnvironment()->KeyGen(), status);
}

grpc::Status ValidatePagination(Pagination const &pagination, unsigned result_per_page_limit) {
    if (pagination.page_number() < 0) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                            "page_number must be >= 0, but got page_number=" +
                                std::to_string(pagination.page_number()));
    }
    if (pagination.result_per_page() <= 0) {
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                            "result_per_page must be > 0, but got result_per_page=" +
                                std::to_string(pagination.result_per_page()));
    }
    if (static_cast<unsigned>(pagination.result_per_page()) > result_per_page_limit) {
        return grpc::Status(
            grpc::StatusCode::INVALID_ARGUMENT,
            "result_per_page_limit=" + std::to_string(result_per_page_limit) +
                ", but got result_per_page=" + std::to_string(pagination.result_per_page()));
    }
    return grpc::Status::OK;
}

} // namespace e8
