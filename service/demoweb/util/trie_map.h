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

#ifndef TRIE_MAP_H
#define TRIE_MAP_H

#include <cassert>
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace e8 {

template <typename KeyElementType, typename NodeType>
using TrieNodes = std::unordered_map<KeyElementType, std::unique_ptr<NodeType>>;

template <typename KeyElementType, typename NodeType>
using TrieNodeIterator = typename TrieNodes<KeyElementType, NodeType>::const_iterator;

/**
 * @brief TrieNode struct The default trie node.
 */
template <typename KeyElementType, typename ValueType> struct TrieNode {
    TrieNodes<KeyElementType, TrieNode> children;
    std::optional<ValueType> value;
};

/**
 * @brief The TrieMap class Implements the map abstract data type using prefix tree.
 * This map does not guarantee thread safety.
 */
template <typename KeyType, typename ValueType,
          typename NodeType = TrieNode<typename KeyType::value_type, ValueType>>
class TrieMap {
  public:
    TrieMap();
    ~TrieMap() = default;
    TrieMap(TrieMap<KeyType, ValueType> const &other);

    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<KeyType, ValueType>;
    using KeyElementType = typename KeyType::value_type;

    /**
     * @brief The Pathlet struct Prefix tree's path element.
     */
    template <typename PathletNodeType> struct Pathlet {
        Pathlet(PathletNodeType *node,
                TrieNodeIterator<KeyElementType, NodeType> const &children_it,
                TrieNodeIterator<KeyElementType, NodeType> const &children_end_it);
        Pathlet(Pathlet const &) = default;

        bool operator==(Pathlet const &rhs) const;
        bool operator!=(Pathlet const &rhs) const;

        PathletNodeType *node;
        TrieNodeIterator<KeyElementType, NodeType> children_it;
        TrieNodeIterator<KeyElementType, NodeType> children_end_it;
    };

    template <typename PathletNodeType, typename IteratorValueType> class iterator_base {
      public:
        iterator_base() = default;

        /**
         * @brief const_iterator Bootstrap from a root node.
         * @param root Root node to bootstrap from.
         */
        iterator_base(Pathlet<PathletNodeType> const &root);

        /**
         * @brief const_iterator
         * @param key
         * @param path
         */
        iterator_base(KeyType const &key, std::vector<Pathlet<PathletNodeType>> const &path);
        iterator_base(iterator_base const &) = default;

        ~iterator_base() = default;

        /**
         * @brief operator ==
         * @param it
         * @return
         */
        bool operator==(iterator_base const &it) const;

        /**
         * @brief operator !=
         * @param it
         * @return
         */
        bool operator!=(iterator_base const &it) const;

        /**
         * @brief operator ++ Prefix increment. Move the iterator to the next element in the trie
         * map. The order of iteration is not alphabetical but that of a pre-order traversal of the
         * prefix tree.
         *
         * @return This iterator which points to the next position.
         */
        iterator_base &operator++();

        /**
         * @brief operator *
         * @return
         */
        std::pair<KeyType const &, IteratorValueType &> const operator*();

        /**
         * @brief current_node
         * @return
         */
        PathletNodeType *node();

        /**
         * @brief node
         * @return
         */
        PathletNodeType const *node() const;

      public:
        KeyType key_;
        std::vector<Pathlet<PathletNodeType>> path_;
    };

    using const_iterator = iterator_base<NodeType const, ValueType const>;
    using iterator = iterator_base<NodeType, ValueType>;

    /**
     * @brief operator =
     * @param rhs
     * @return
     */
    TrieMap<KeyType, ValueType> &operator=(TrieMap<KeyType, ValueType> rhs);

    /**
     * @brief operator ==
     * @param rhs
     * @return
     */
    bool operator==(TrieMap<KeyType, ValueType> const &rhs) const;

    /**
     * @brief operator !=
     * @param rhs
     * @return
     */
    bool operator!=(TrieMap<KeyType, ValueType> const &rhs) const;

    /**
     * @brief operator []
     * @param key
     * @return
     */
    ValueType &operator[](KeyType const &key);

    /**
     * @brief operator []
     * @param key
     * @return
     */
    ValueType operator[](KeyType const &key) const;

    /**
     * @brief size
     * @return
     */
    size_t size() const;

    /**
     * @brief empty
     * @return
     */
    bool empty() const;

    /**
     * @brief clear
     */
    void clear();

    /**
     * @brief find
     * @param key
     * @return
     */
    iterator find(KeyType const &key);

    /**
     * @brief find
     * @param key
     * @return
     */
    const_iterator find(KeyType const &key) const;

    /**
     * @brief insert
     * @param entry
     * @return
     */
    std::pair<iterator, bool> insert(std::pair<KeyType, ValueType> const &entry);

    /**
     * @brief erase
     * @param it
     */
    void erase(iterator it);

    /**
     * @brief begin
     * @return
     */
    iterator begin();

    /**
     * @brief begin
     * @return
     */
    const_iterator begin() const;

    /**
     * @brief end
     * @return
     */
    iterator end();

    /**
     * @brief end
     * @return
     */
    const_iterator end() const;

  protected:
    template <typename PathletNodeType>
    static Pathlet<PathletNodeType> *NextPathlet(KeyType *key,
                                                 std::vector<Pathlet<PathletNodeType>> *path);
    template <typename PathletNodeType>
    static void NextKeyPath(KeyType *key, std::vector<Pathlet<PathletNodeType>> *path);

    template <typename PathletNodeType> struct PathInfo {
        std::vector<Pathlet<PathletNodeType>> path;
        typename KeyType::const_iterator key_it;
        bool matches_key = false;
    };

    template <typename PathletNodeType, typename TrieNodeType>
    static PathInfo<PathletNodeType> FindPath(KeyType const &key, TrieNodeType *root);

    std::unique_ptr<NodeType> root_;
    size_t num_elements_ = 0;
};

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType>
TrieMap<KeyType, ValueType, NodeType>::Pathlet<PathletNodeType>::Pathlet(
    PathletNodeType *node, TrieNodeIterator<KeyElementType, NodeType> const &children_it,
    TrieNodeIterator<KeyElementType, NodeType> const &children_end_it)
    : node(node), children_it(children_it), children_end_it(children_end_it) {}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType>
bool TrieMap<KeyType, ValueType, NodeType>::Pathlet<PathletNodeType>::operator==(
    Pathlet const &rhs) const {
    return node == rhs.node;
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType>
bool TrieMap<KeyType, ValueType, NodeType>::Pathlet<PathletNodeType>::operator!=(
    Pathlet const &rhs) const {
    return !(*this == rhs);
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType>
typename TrieMap<KeyType, ValueType, NodeType>::template Pathlet<PathletNodeType> *
TrieMap<KeyType, ValueType, NodeType>::NextPathlet(KeyType *key,
                                                   std::vector<Pathlet<PathletNodeType>> *path) {
    assert(!path->empty());

    Pathlet<PathletNodeType> *cur_pathlet = &path->back();
    while (cur_pathlet->children_it == cur_pathlet->children_end_it) {
        key->pop_back();
        path->pop_back();
        if (path->empty()) {
            return nullptr;
        }
        cur_pathlet = &path->back();
        ++cur_pathlet->children_it;
    }

    auto const &[key_elm, node] = *cur_pathlet->children_it;
    key->push_back(key_elm);
    path->push_back(Pathlet(node.get(), node->children.begin(), node->children.end()));

    return &path->back();
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType>
void TrieMap<KeyType, ValueType, NodeType>::NextKeyPath(
    KeyType *key, std::vector<Pathlet<PathletNodeType>> *path) {
    assert(!path->empty());

    Pathlet<PathletNodeType> *cur_pathlet;
    while ((cur_pathlet = TrieMap<KeyType, ValueType, NodeType>::NextPathlet(key, path)) !=
               nullptr &&
           !cur_pathlet->node->value.has_value()) {
    }
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
TrieMap<KeyType, ValueType, NodeType>::iterator_base<
    PathletNodeType, IteratorValueType>::iterator_base(Pathlet<PathletNodeType> const &root) {
    path_.push_back(root);
    if (root.node != nullptr && !root.node->value.has_value()) {
        // Need to move to the first node that forms a complete key.
        TrieMap<KeyType, ValueType, NodeType>::NextKeyPath(&key_, &path_);
    }
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType, IteratorValueType>::
    iterator_base(KeyType const &key, std::vector<Pathlet<PathletNodeType>> const &path)
    : key_(key), path_(path) {}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
bool TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType, IteratorValueType>::
operator==(iterator_base<PathletNodeType, IteratorValueType> const &it) const {
    if (path_.size() != it.path_.size()) {
        return false;
    }
    // When non-empty, two iterators are equal if their currently visiting node is the same.
    return path_.empty() || path_.back() == it.path_.back();
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
bool TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType, IteratorValueType>::
operator!=(iterator_base<PathletNodeType, IteratorValueType> const &it) const {
    return !(*this == it);
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
typename TrieMap<KeyType, ValueType, NodeType>::template iterator_base<PathletNodeType,
                                                                       IteratorValueType> &
TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType,
                                                     IteratorValueType>::operator++() {
    this->NextValuePathFrom(&key_, &path_);
    return *this;
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
std::pair<KeyType const &, IteratorValueType &> const
    TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType,
                                                         IteratorValueType>::operator*() {
    assert(!path_.empty());
    std::optional<IteratorValueType> &val = path_.back().node->value;
    assert(val.has_value());
    return std::pair<KeyType const &, IteratorValueType &>(key_, val.value());
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
PathletNodeType *
TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType, IteratorValueType>::node() {
    assert(!path_.empty());
    assert(!path_.back().node->value.has_value());
    return path_.back().node;
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename IteratorValueType>
PathletNodeType const *
TrieMap<KeyType, ValueType, NodeType>::iterator_base<PathletNodeType, IteratorValueType>::node()
    const {
    assert(!path_.empty());
    assert(!path_.back().node->value.has_value());
    return path_.back().node;
}

template <typename KeyType, typename ValueType, typename NodeType>
TrieMap<KeyType, ValueType, NodeType>::TrieMap() : root_(std::make_unique<NodeType>()) {}

template <typename KeyType, typename ValueType, typename NodeType>
TrieMap<KeyType, ValueType, NodeType>::TrieMap(TrieMap<KeyType, ValueType> const &other) {}

template <typename KeyType, typename ValueType, typename NodeType>
size_t TrieMap<KeyType, ValueType, NodeType>::size() const {
    return num_elements_;
}

template <typename KeyType, typename ValueType, typename NodeType>
bool TrieMap<KeyType, ValueType, NodeType>::empty() const {
    return size() == 0;
}

template <typename KeyType, typename ValueType, typename NodeType>
void TrieMap<KeyType, ValueType, NodeType>::clear() {
    root_.clear();
    num_elements_ = 0;
}

template <typename KeyType, typename ValueType, typename NodeType>
template <typename PathletNodeType, typename TrieNodeType>
typename TrieMap<KeyType, ValueType, NodeType>::template PathInfo<PathletNodeType>
TrieMap<KeyType, ValueType, NodeType>::FindPath(KeyType const &key, TrieNodeType *root) {
    PathInfo<PathletNodeType> path_info;

    PathletNodeType *cur_node = root;
    for (path_info.key_it = key.begin(); path_info.key_it != key.end(); ++path_info.key_it) {
        auto it = cur_node->children.find(*path_info.key_it);
        path_info.path.push_back(Pathlet<PathletNodeType>(cur_node, it, cur_node->children.end()));
        if (it == cur_node->children.end()) {
            return path_info;
        }

        cur_node = it->second.get();
    }
    path_info.path.push_back(
        Pathlet<PathletNodeType>(cur_node, cur_node->children.begin(), cur_node->children.end()));

    assert(cur_node != nullptr);
    if (!cur_node->value.has_value()) {
        return path_info;
    }

    path_info.matches_key = true;
    return path_info;
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::iterator
TrieMap<KeyType, ValueType, NodeType>::find(KeyType const &key) {
    auto path_info = TrieMap::FindPath<NodeType>(key, root_.get());
    if (!path_info.matches_key) {
        return this->end();
    }
    return iterator(key, path_info.path);
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::const_iterator
TrieMap<KeyType, ValueType, NodeType>::find(KeyType const &key) const {
    auto path_info = TrieMap::FindPath<NodeType const>(key, root_.get());
    if (!path_info.matches_key) {
        return this->end();
    }
    return const_iterator(key, path_info.path);
}

template <typename KeyType, typename ValueType, typename NodeType>
std::pair<typename TrieMap<KeyType, ValueType, NodeType>::iterator, bool>
TrieMap<KeyType, ValueType, NodeType>::insert(std::pair<KeyType, ValueType> const &entry) {
    auto const &[key, value] = entry;

    auto path_info = TrieMap::FindPath<NodeType>(key, root_.get());
    if (path_info.matches_key) {
        path_info.path.back().node->value = value;
        return std::make_pair(iterator(key, path_info.path), false);
    }

    for (; path_info.key_it != key.end(); ++path_info.key_it) {
        auto &last_pathlet = path_info.path.back();
        auto exterior_node = last_pathlet.node;
        auto new_node_it = exterior_node->children.insert(
            std::make_pair(*path_info.key_it, std::make_unique<NodeType>()));
        last_pathlet.children_it = new_node_it.first;
        last_pathlet.children_end_it = exterior_node->children.end();

        auto inserted_node = new_node_it.first->second.get();
        path_info.path.push_back(Pathlet<NodeType>(inserted_node, exterior_node->children.begin(),
                                                   exterior_node->children.end()));
    }
    path_info.path.back().node->value = value;

    ++num_elements_;
    return std::make_pair(iterator(key, path_info.path), true);
}

template <typename KeyType, typename ValueType, typename NodeType>
void TrieMap<KeyType, ValueType, NodeType>::erase(iterator it) {
    assert(it != this->end());

    auto node = it.path_.back().node;
    assert(node->value.has_value());
    node->value = std::nullopt;

    while (true) {
        if (node->children.empty() && node->value.has_value()) {
            break;
        }

        it.path_.pop_back();
        if (it.path_.empty()) {
            break;
        }

        node = it.path_.back().node;
        auto children_it = it.path_.back().children_it;
        assert(children_it != it.path_.back().children_end_it);
        node->children.erase(children_it);
    }

    --num_elements_;
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::iterator
TrieMap<KeyType, ValueType, NodeType>::begin() {
    return iterator(Pathlet<NodeType>(root_.get(), root_->children.begin(), root_.children.end()));
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::const_iterator
TrieMap<KeyType, ValueType, NodeType>::begin() const {
    return const_iterator(
        Pathlet<NodeType const>(root_.get(), root_->children.begin(), root_.children.end()));
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::iterator
TrieMap<KeyType, ValueType, NodeType>::end() {
    return iterator(
        Pathlet<NodeType>(/*node=*/nullptr, root_->children.end(), root_->children.end()));
}

template <typename KeyType, typename ValueType, typename NodeType>
typename TrieMap<KeyType, ValueType, NodeType>::const_iterator
TrieMap<KeyType, ValueType, NodeType>::end() const {
    return const_iterator(
        Pathlet<NodeType const>(/*node=*/nullptr, root_->children.end(), root_->children.end()));
}

} // namespace e8

#endif // TRIE_MAP_H
