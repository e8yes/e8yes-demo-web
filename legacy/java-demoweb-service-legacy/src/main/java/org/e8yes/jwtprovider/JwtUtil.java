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
package org.e8yes.jwtprovider;

import java.util.Base64;

public class JwtUtil {

  /**
   * This is done to reduce the size of the JWT token, since byte array takes less space than base64
   * string.
   *
   * @param jwtToken JWT token encoded in base64 format.
   * @return JWT byte array.
   */
  public static byte[] convertToByteArray(String jwtToken) {
    // Split into header, payload and signature.
    String[] parts = jwtToken.split("\\.");
    assert (parts.length == 3);
    byte[] header = Base64.getUrlDecoder().decode(parts[0]);
    byte[] payload = Base64.getUrlDecoder().decode(parts[1]);
    byte[] signature = Base64.getUrlDecoder().decode(parts[2]);
    int headerLength = header.length;
    int payloadLength = payload.length;
    assert (headerLength < (1 << 16));
    assert (payloadLength < (1 << 16));
    byte[] finalToken = new byte[4 + header.length + payload.length + signature.length];
    // Write the segment length information into the first 2 bytes in little-endian format.
    finalToken[0] = (byte) headerLength;
    finalToken[1] = (byte) (headerLength >>> 8);
    finalToken[2] = (byte) payloadLength;
    finalToken[3] = (byte) (payloadLength >>> 8);
    System.arraycopy(header, 0, finalToken, 4, headerLength);
    System.arraycopy(payload, 0, finalToken, 4 + headerLength, payloadLength);
    System.arraycopy(signature, 0, finalToken, 4 + headerLength + payloadLength, signature.length);
    return finalToken;
  }

  /**
   * JWT verifier has to ingest the standard base64 string format. It converts from a compact byte
   * array back to the standard base64 format.
   *
   * @param jwtBytes Compact byte array.
   * @return standard base64 format.
   */
  public static String convertFromByteArray(byte[] jwtBytes) {
    int headerLength = jwtBytes[0] | (jwtBytes[1] << 8);
    int payloadLength = jwtBytes[2] | (jwtBytes[3] << 8);
    int signatureLength = jwtBytes.length - headerLength - payloadLength - 4;

    byte[] header = new byte[headerLength];
    byte[] payload = new byte[payloadLength];
    byte[] signature = new byte[signatureLength];
    System.arraycopy(jwtBytes, 4, header, 0, headerLength);
    System.arraycopy(jwtBytes, 4 + headerLength, payload, 0, payloadLength);
    System.arraycopy(jwtBytes, 4 + headerLength + payloadLength, signature, 0, signature.length);

    String b64Header = Base64.getUrlEncoder().withoutPadding().encodeToString(header);
    String b64Payload = Base64.getUrlEncoder().withoutPadding().encodeToString(payload);
    String b64Signature = Base64.getUrlEncoder().withoutPadding().encodeToString(signature);

    return String.format("%s.%s.%s", b64Header, b64Payload, b64Signature);
  }
}
