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

#ifndef MESSAGE_QUEUE_DISTRIBUTOR_H
#define MESSAGE_QUEUE_DISTRIBUTOR_H

#include <memory>

#include "distributor/distributor/distribute.h"

namespace e8 {

/**
 * @brief CreateMessageQueueDistributor Create a distributor which the message queue module should
 * use.
 */
std::unique_ptr<DistributorInterface> CreateMessageQueueDistributor();

} // namespace e8

#endif // MESSAGE_QUEUE_DISTRIBUTOR_H
