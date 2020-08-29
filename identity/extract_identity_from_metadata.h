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

#ifndef EXTRACT_IDENTITY_FROM_METADATA_H
#define EXTRACT_IDENTITY_FROM_METADATA_H

#include <grpcpp/grpcpp.h>
#include <optional>
#include <string>

#include "keygen/key_generator_interface.h"
#include "proto_cc/identity.pb.h"

namespace e8 {

/**
 * @brief ExtractIdentityFromContext Utility function to validate and decode user identity from the
 * server context.
 *
 * @param auth_key Key of the signature that is expected to have set into the context.
 * @param key_gen Key generator that holds the key to decode the signature.
 * @param status The status of the extraction (nullable). If there is no error, the extracted
 * identity will be returned. Otherwise, a nullopt will be returned.
 * @return The decoded identity if no error.
 */
std::optional<Identity> ExtractIdentityFromContext(grpc::ServerContext const &context,
                                                   std::string const &auth_key,
                                                   KeyGeneratorInterface *key_gen,
                                                   grpc::Status *status);

} // namespace e8

#endif // EXTRACT_IDENTITY_FROM_METADATA_H
