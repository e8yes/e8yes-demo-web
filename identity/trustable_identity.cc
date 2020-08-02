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

#include <ctime>
#include <optional>
#include <string>

#include "identity/trustable_identity.h"
#include "keygen/key_generator_interface.h"
#include "keygen/sign_message.h"
#include "proto_cc/identity.pb.h"
#include "third_party/base64/base64.h"

namespace e8 {
namespace {

static char const kEncrypter[] = "IdentitySigner";

} // namespace

std::optional<SignedIdentity> SignIdentity(Identity const &identity,
                                           KeyGeneratorInterface *key_gen) {
    std::string identity_bytes;
    bool serialize_status = identity.SerializeToString(&identity_bytes);
    assert(serialize_status == true);

    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);

    std::string raw_bytes = SignMessage(identity_bytes, key_pair.key);
    return base64_encode(raw_bytes);
}

std::optional<Identity> ValidateSignedIdentity(SignedIdentity const &signed_identity,
                                               KeyGeneratorInterface *key_gen) {
    KeyGeneratorInterface::Key key_pair =
        key_gen->KeyOf(kEncrypter, KeyGeneratorInterface::RSA_4096_BITS);
    assert(key_pair.public_key.has_value());

    std::string raw_bytes = base64_decode(signed_identity);

    std::optional<std::string> decoded_bytes =
        DecodeSignedMessage(raw_bytes, key_pair.public_key.value());
    if (!decoded_bytes.has_value()) {
        return std::nullopt;
    }

    Identity identity;
    bool deserialize_status =
        identity.ParseFromArray(decoded_bytes.value().data(), decoded_bytes.value().size());
    assert(deserialize_status == true);

    std::time_t cur_timestamp;
    std::time(&cur_timestamp);
    if (cur_timestamp > identity.expiry_timestamp()) {
        return std::nullopt;
    }

    return identity;
}

} // namespace e8
