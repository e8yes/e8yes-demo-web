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

#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace e8 {

/**
 * @brief The RandomSource class Provides a source of pseudo-randomness.
 */
class RandomSource {
  public:
    /**
     * @brief RandomSource Automatically select a random seed.
     */
    RandomSource();

    /**
     * @brief RandomSource Select a fixed sequence of random number by providing a random seed.
     */
    explicit RandomSource(unsigned seed);

    /**
     * @brief Draw A uniformly distributed random number between [0, 1).
     */
    float Draw();

  private:
    std::mt19937 engine_;
    std::uniform_real_distribution<float> uniform_;
};

} // namespace e8

#endif // RANDOM_H
