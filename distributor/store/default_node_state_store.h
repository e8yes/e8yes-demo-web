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

#ifndef DEFAULT_NODE_STATE_STORE_H
#define DEFAULT_NODE_STATE_STORE_H

#include <memory>
#include <string>

#include "distributor/store/node_state_store.h"

namespace e8 {

/**
 * @brief InitDefaultNodeStateStoreProvider Create the node state store if it hasn't been. Only the
 * first call will take any actual effect. This function must be called at least once before using
 * DefaultNodeStateStore().
 */
void InitDefaultNodeStateStoreProvider(std::string const &node_state_db_path);

/**
 * @brief DefaultNodeStateStore The default node state store that was created by calling
 * InitDefaultNodeStateStoreProvider().
 */
std::shared_ptr<NodeStateStoreInterface> DefaultNodeStateStore();

} // namespace e8

#endif // DEFAULT_NODE_STATE_STORE_H
