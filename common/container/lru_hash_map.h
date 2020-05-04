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

#ifndef LRU_TRIE_MAP_H
#define LRU_TRIE_MAP_H

#include <cassert>
#include <list>
#include <memory>
#include <utility>

#include "common/container/trie_map.h"

namespace e8 {
namespace {
/**
 * @brief LruCacheItem struct A trie node list element ordered by cache hit recency.
 */
template <typename KeyType, typename ValueType> struct LruCacheItem {
    LruCacheItem(KeyType const &key, ValueType const &value);

    KeyType key; // TODO: Avoid storing an extra key here. It should use a pointer to the hash map's
                 // location instead.
    ValueType value;
    LruCacheItem *next = nullptr;
    LruCacheItem *prev = nullptr;
};

template <typename KeyType, typename ValueType>
LruCacheItem<KeyType, ValueType>::LruCacheItem(KeyType const &key, ValueType const &value)
    : key(key), value(value) {}

} // namespace

/**
 * @brief The LruHashMap class An in-memory least-recently used cache, for which the underlying
 * key-value implementation uses a hash map. This cache does not guarantee thread safety.
 */
template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
class LruHashMap {
  public:
    /**
     * @brief LruHashMap Constructs an empty cache.
     *
     * @param max_size The maximum number of items can be stored in the cache.
     * @param on_fetch A functor-like object that takes the key and returns the fetched value.
     * Example signature: ValueType(KeyType const&). The functor will be used when the key isn't
     * present in the cache.
     * @param on_evict A functor-like object that takes the value and release relevant resources.
     * Example signature: void(ValueType const&). The functor will be used when a cached item needs
     * to be evicted from the cache.
     */
    LruHashMap(uint32_t max_size, OnFetch const &on_fetch, OnEvict const &on_evict);
    LruHashMap(LruHashMap const &other) = delete;
    ~LruHashMap() = default;

    /**
     * @brief Fetch Fetches the value of an item which associates with the key.
     * If the key doesn't exist in the cache, it will load the item into the cache. If the cache
     * exists the size limit, LRU item will be evicted.
     *
     * @param key Unique key that maps to the value.
     * @return The value mapped by the key.
     */
    ValueType const &Fetch(KeyType const &key);

    /**
     * @brief clear Empties the cache to its original empty state. All the existing values will be
     * evicted.
     */
    void Clear();

    /**
     * @brief FetchOperator For testing purpose, it's useful to be able to return the fetch
     * operator.
     * @return The fetch functor.
     */
    OnFetch const &FetchOperator() const;

    /**
     * @brief EvictOperator For testing purpose, it's useful to be able to return the eviction
     * operator.
     * @return The eviction functor.
     */
    OnEvict const &EvictOperator() const;

  private:
    using Item = LruCacheItem<KeyType, ValueType>;

    Item *CacheNewItem(KeyType const &key, ValueType const &value);
    void SetMostRecentlyUsed(Item *most_recent_node);

    std::unordered_map<KeyType, std::unique_ptr<Item>> cache_;

    Item *head_ = nullptr;
    Item *tail_ = nullptr;

    uint32_t max_size_;
    int32_t padding_;

    OnFetch on_fetch_;
    OnEvict on_evict_;
};

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
typename LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::Item *
LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::CacheNewItem(KeyType const &key,
                                                               ValueType const &value) {
    if (cache_.size() == max_size_) {
        // Need to remove the least frequently used item.
        Item *to_be_removed = tail_;

        // There is no need to handle the head pointer because it's assumed that max_size_ must be
        // greater than 1.
        tail_ = tail_->prev;

        on_evict_(to_be_removed->value);
        cache_.erase(to_be_removed->key);
    }
    assert(cache_.size() < max_size_);

    auto insertion = cache_.insert(std::make_pair(key, std::make_unique<Item>(key, value)));
    assert(insertion.second == true);
    Item *item = insertion.first->second.get();

    if (head_ == nullptr) {
        head_ = item;
        tail_ = item;
    } else {
        item->next = head_;
        head_->prev = item;
        head_ = item;
    }

    return item;
}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
void LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::SetMostRecentlyUsed(Item *most_recent_node) {
    if (most_recent_node->prev == nullptr) {
        // It has already been the most recently used item.
        return;
    }
    if (most_recent_node->next != nullptr) {
        most_recent_node->prev->next = most_recent_node->next;
        most_recent_node->next->prev = most_recent_node->prev;
    } else {
        most_recent_node->prev->next = nullptr;
        tail_ = most_recent_node->prev;
    }
    most_recent_node->prev = nullptr;
    most_recent_node->next = head_;
    head_->prev = most_recent_node;
    head_ = most_recent_node;
}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::LruHashMap(uint32_t max_size,
                                                             OnFetch const &on_fetch,
                                                             OnEvict const &on_evict)
    : max_size_(max_size), on_fetch_(on_fetch), on_evict_(on_evict) {}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
ValueType const &LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::Fetch(KeyType const &key) {
    Item *item;

    auto it = cache_.find(key);
    if (it != cache_.end()) {
        // Cache hit.
        item = it->second.get();
        this->SetMostRecentlyUsed(item);
    } else {
        // Cache miss.
        ValueType value = on_fetch_(key);
        item = this->CacheNewItem(key, value);
    }

    return item->value;
}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
void LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::Clear() {
    for (auto const &[key, item] : cache_) {
        on_evict_(item->value);
    }
    cache_.clear();

    head_ = nullptr;
    tail_ = nullptr;
}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
OnFetch const &LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::FetchOperator() const {
    return on_fetch_;
}

template <typename KeyType, typename ValueType, typename OnFetch, typename OnEvict>
OnEvict const &LruHashMap<KeyType, ValueType, OnFetch, OnEvict>::EvictOperator() const {
    return on_evict_;
}

} // namespace e8

#endif // LRU_TRIE_MAP_H
