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

#ifndef KEY_GENERATOR_INTERFACE_H
#define KEY_GENERATOR_INTERFACE_H

#include <string>

namespace e8 {

/**
 * @brief The KeyGeneratorInterface class For generating dynamic encryption key. This avoids having
 * to store key files statically. Implementation of this interface must be thread safe.
 */
class KeyGeneratorInterface {
  public:
    KeyGeneratorInterface() = default;
    KeyGeneratorInterface(KeyGeneratorInterface const &) = delete;
    virtual ~KeyGeneratorInterface() = default;

    /**
     * @brief KeyOf Retrieve an encryption key for the specified encrypter. If a key doesn't exist,
     * it will generete one for the encrypter.
     * @param encrypter Name of the encrypter for which the key needs to be returned.
     * @return Dynamically generated encrpytion key for the encrypter.
     */
    virtual std::string KeyOf(std::string const &encrypter) = 0;
};

} // namespace e8

#endif // KEY_GENERATOR_INTERFACE_H
