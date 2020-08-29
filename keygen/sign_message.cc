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

#include <cryptopp/config.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <optional>
#include <string>

#include "keygen/sign_message.h"

namespace e8 {
namespace {

static CryptoPP::AutoSeededRandomPool gRng;

} // namespace

std::string SignMessage(std::string const &message_bytes, std::string const &raw_private_key) {
    CryptoPP::StringSource private_key_source(raw_private_key, /*pumpAll=*/true);
    CryptoPP::RSA::PrivateKey private_key;
    private_key.Load(private_key_source);

    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer(private_key);

    std::string signature;
    CryptoPP::StringSource message_source(
        message_bytes, true,
        new CryptoPP::SignerFilter(gRng, signer, new CryptoPP::StringSink(signature), true));

    return signature;
}

std::optional<std::string> DecodeSignedMessage(std::string const &signed_message_bytes,
                                               std::string const &raw_public_key) {
    CryptoPP::StringSource public_key_source(raw_public_key, /*pumpAll=*/true);
    CryptoPP::RSA::PublicKey public_key;
    public_key.Load(public_key_source);

    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Verifier verifier(public_key);

    std::string recovered;

    try {
        CryptoPP::StringSource signed_message_source(
            signed_message_bytes, true,
            new CryptoPP::SignatureVerificationFilter(
                verifier, new CryptoPP::StringSink(recovered),
                CryptoPP::SignatureVerificationFilter::THROW_EXCEPTION |
                    CryptoPP::SignatureVerificationFilter::PUT_MESSAGE));
    } catch (CryptoPP::SignatureVerificationFilter::SignatureVerificationFailed const &) {
        return std::nullopt;
    }

    return recovered;
}

} // namespace e8
