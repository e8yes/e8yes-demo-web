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

#ifndef SERVICE_UTIL_H
#define SERVICE_UTIL_H

#include <grpcpp/grpcpp.h>
#include <optional>

#include "demoweb_service/demoweb/proto_cc/identity.pb.h"
#include "demoweb_service/demoweb/proto_cc/pagination.pb.h"

namespace e8 {

/**
 * @brief ExtractIdentityFromContext Utility function to validate and decode user identity from the
 * server context.
 *
 * @status The status of the extraction (nullable). If there is no error, the extracted identity
 * will be returned. Otherwise, a nullopt will be returned.
 */
std::optional<Identity> ExtractIdentityFromContext(grpc::ServerContext const &context,
                                                   grpc::Status *status);

/**
 * @brief ValidatePagination Validate the parameters of a pagination object.
 *
 * @param pagination The pagination object to be validated.
 * @param result_per_page_limit Inclusive limit of the result_per_page parameter.
 * @return OK status if no error found.
 */
grpc::Status ValidatePagination(Pagination const &pagination, unsigned result_per_page_limit);

} // namespace e8

#endif // SERVICE_UTIL_H
