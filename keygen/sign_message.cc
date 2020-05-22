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

#include <cryptopp/config.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cstdint>
#include <optional>
#include <vector>

#include "keygen/sign_message.h"

namespace e8 {
namespace {

static CryptoPP::AutoSeededRandomPool gRng;

} // namespace

std::vector<uint8_t> SignMessage(std::vector<uint8_t> const &message_bytes,
                                 std::vector<uint8_t> const &raw_private_key) {
    CryptoPP::ArraySource private_key_source(raw_private_key.data(), raw_private_key.size(),
                                             /*pumpAll=*/true);
    CryptoPP::RSA::PrivateKey private_key;
    private_key.Load(private_key_source);

    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Signer signer(private_key);
    CryptoPP::SecByteBlock signature(signer.MaxSignatureLength(message_bytes.size()));
    size_t signature_len = signer.SignMessageWithRecovery(
        gRng, message_bytes.data(), message_bytes.size(), nullptr, 0, signature);
    signature.resize(signature_len);

    return std::vector<uint8_t>(signature.begin(), signature.end());
}

std::optional<std::vector<uint8_t>>
DecodeSignedMessage(std::vector<uint8_t> const &signed_message_bytes,
                    std::vector<uint8_t> const &raw_public_key) {
    CryptoPP::ArraySource public_key_source(raw_public_key.data(), raw_public_key.size(),
                                            /*pumpAll=*/true);
    CryptoPP::RSA::PublicKey public_key;
    public_key.Load(public_key_source);

    CryptoPP::RSASS<CryptoPP::PSSR, CryptoPP::SHA256>::Verifier verifier(public_key);
    CryptoPP::SecByteBlock recovered(
        verifier.MaxRecoverableLengthFromSignatureLength(signed_message_bytes.size()));

    CryptoPP::SecByteBlock signature(signed_message_bytes.data(), signed_message_bytes.size());
    CryptoPP::DecodingResult result =
        verifier.RecoverMessage(recovered, nullptr, 0, signature, signed_message_bytes.size());

    if (!result.isValidCoding) {
        return std::nullopt;
    }

    recovered.resize(result.messageLength);

    return std::vector<uint8_t>(recovered.begin(), recovered.end());
}

} // namespace e8
