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

#ifndef FIXEDPRIORITYQUEUE_H
#define FIXEDPRIORITYQUEUE_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

namespace e8 {

/**
 * @brief The MutablePriorityQueue class A priority queue that allows elements to change priority
 * and to be removed. It does not guarantee thread-safety.
 */
template <typename ValueType, typename LessThan = std::less<ValueType>> class MutablePriorityQueue {
  private:
    struct Node {
        size_t idx;
        ValueType value;

        Node(size_t idx, ValueType const &value);
    };

  public:
    MutablePriorityQueue() = default;
    ~MutablePriorityQueue() = default;

    template <typename HeapType, typename IteratorValueType> class iterator_base {
      public:
        ~iterator_base() = default;

        /**
         * @brief iterator_base
         *
         * @param heap_type
         * @param current_node
         */
        iterator_base(HeapType *heap, Node *current_node);

        /**
         * @brief operator == Two iterators are equal if they have the same tree path.
         *
         * @param rhs Another iterator to compare against.
         * @return True if iterator rhs is equal to this iterator.
         */
        bool operator==(iterator_base const &rhs) const;

        /**
         * @brief operator != Negation of the equality operator.
         *
         * @param rhs Another iterator to compare against.
         * @return True if iterator rhs does not equal to this iterator.
         */
        bool operator!=(iterator_base const &rhs) const;

        /**
         * @brief operator ++ Prefix increment. Moves the iterator to the next element. Note that,
         * the order of travesal is not bound to the element's priority.
         *
         * @return This iterator which points to the next position.
         */
        iterator_base &operator++();

        /**
         * @brief operator * de-references operator.
         *
         * @return value at the current the iterator position.
         */
        IteratorValueType &operator*();

      private:
        Node *current_node_ = nullptr;
        HeapType *heap_;
    };

    using const_iterator = iterator_base<std::vector<std::unique_ptr<Node>> const, ValueType const>;
    using iterator = iterator_base<std::vector<std::unique_ptr<Node>> const, ValueType>;

    /**
     * @brief add Ignores the priority of the element and adds the value to the container. This is
     * helpful because it allows a single make_heap() call after all elements are added to the
     * container.
     *
     * @param value Element to be added to the container.
     */
    void add(ValueType const &value);

    /**
     * @brief make_heap Reprioritizes all the elements in the container in linear time. It will not
     * invalidate any iterators.
     */
    void make_heap();

    /**
     * @brief reprioritize Reprioritizes the element at the iterator's position. It will not
     * invalidate any iterators.
     */
    void reprioritize(iterator &it);

    /**
     * @brief erase Remove the element at the iterator's position. It will invalidate the iterator
     * that is going to be removed. After the removal, the iterator will point to the next element
     * after it.
     */
    void erase(iterator &it);

    /**
     * @brief push Adds the element to the queue and insert it at the position where it's greater
     * than all others after it. It will not invalidate any iterators.
     *
     * @param value The element to te inserted.
     */
    iterator push(ValueType const &value);

    /**
     * @brief pop Removes the element returned by top(). It will not invalidate any iterators.
     */
    void pop();

    /**
     * @brief clear Removes all elements in the queue. It will invalidate any iterators.
     */
    void clear();

    /**
     * @brief top Returns the element with the highest priority. If there are multiple elements with
     * the highest priority, one of them is chosen deterministically.
     */
    ValueType const &top() const;

    /**
     * @brief size Returns the total number elements in the container.
     */
    size_t size() const;

    /**
     * @brief empty Checks if the container is empty.
     */
    bool empty() const;

    /**
     * @brief front Returns the iterator pointing to the top() element.
     */
    iterator front();

    /**
     * @brief front Constant version of the above function.
     */
    const_iterator front() const;

    /**
     * @brief begin Returns an iterator pointing to the first entry in queue. The first entry
     * happens to be iterator pointing to the top() element. The other iterator positions have
     * nothing to do with the priority of the elements.
     */
    iterator begin();

    /**
     * @brief begin Constant version of the above function.
     */
    const_iterator begin() const;

    /**
     * @brief end Returns an iterator pointing to the next position after the last element in the
     * queue.
     */
    iterator end();

    /**
     * @brief end Constant version of the above function.
     */
    const_iterator end() const;

  private:
    void trickle_up(size_t node_idx);
    void trickle_down(size_t node_idx);

    std::vector<std::unique_ptr<Node>> heap_;
    size_t heap_len_ = 1;
};

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::add(ValueType const &value) {
    heap_.push_back(std::make_unique<Node>(heap_.size(), value));
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::make_heap() {
    if (this->empty()) {
        return;
    }

    heap_len_ = 1;
    while (heap_len_ < heap_.size()) {
        heap_len_ <<= 1;
    }

    for (size_t i = (heap_len_ >> 1) - 1; i >= 0; --i) {
        this->trickle_down(i);
    }
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::reprioritize(iterator &it) {
    assert(it.current_node_ != nullptr);
    this->trickle_up(it.current_node_->idx);
    this->trickle_down(it.current_node_->idx);
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::iterator
MutablePriorityQueue<ValueType, LessThan>::push(ValueType const &value) {
    unsigned current_idx = heap_.size();
    heap_.push_back(std::make_unique<Node>(current_idx, value));

    if (heap_.size() > heap_len_) {
        heap_len_ <<= 1;
    }

    this->trickle_up(current_idx);
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::erase(iterator &it) {
    Node *current_node = it.current_node_;
    ++it;

    // Remove the node lazily.
    assert(current_node != nullptr);
    heap_[current_node->idx] = nullptr;
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::pop() {
    assert(!this->empty());

    while (heap_.back() == nullptr) {
        heap_.pop_back();

        if (heap_.size() <= (heap_len_ >> 1)) {
            heap_len_ >>= 1;
        }
    }

    if (heap_.empty()) {
        heap_len_ = 1;
        return;
    }
    if (heap_.size() == 1) {
        heap_.pop_back();
        return;
    }

    heap_[0] = std::move(heap_.back());
    heap_.pop_back();
    if (heap_.size() <= (heap_len_ >> 1)) {
        heap_len_ >>= 1;
    }

    this->trickle_down(0);
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::clear() {
    heap_.clear();
    heap_len_ = 1;
}

template <typename ValueType, typename LessThan>
ValueType const &MutablePriorityQueue<ValueType, LessThan>::top() const {
    assert(!this->empty());
    return heap_[0]->value;
}

template <typename ValueType, typename LessThan>
size_t MutablePriorityQueue<ValueType, LessThan>::size() const {
    return heap_.size();
}

template <typename ValueType, typename LessThan>
bool MutablePriorityQueue<ValueType, LessThan>::empty() const {
    return heap_.empty();
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::iterator
MutablePriorityQueue<ValueType, LessThan>::front() {
    assert(this->begin() != this->end());
    return this->begin();
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::const_iterator
MutablePriorityQueue<ValueType, LessThan>::front() const {
    assert(this->begin() != this->end());
    return this->begin();
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::iterator
MutablePriorityQueue<ValueType, LessThan>::begin() {
    if (this->empty()) {
        return this->end();
    }
    return iterator(&heap_, heap_[0].get());
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::const_iterator
MutablePriorityQueue<ValueType, LessThan>::begin() const {
    if (this->empty()) {
        return this->end();
    }
    return const_iterator(&heap_, heap_[0].get());
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::iterator
MutablePriorityQueue<ValueType, LessThan>::end() {
    return iterator(&heap_, /*current_node=*/nullptr);
}

template <typename ValueType, typename LessThan>
typename MutablePriorityQueue<ValueType, LessThan>::const_iterator
MutablePriorityQueue<ValueType, LessThan>::end() const {
    return const_iterator();
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::trickle_up(size_t node_idx) {
    if (node_idx == 0) {
        return;
    }

    size_t parent_idx = node_idx >> 1;

    if (LessThan(heap_[parent_idx]->value, heap_[node_idx]->value)) {
        std::swap(heap_[node_idx]->idx, heap_[parent_idx]->idx);
        std::swap(heap_[node_idx], heap_[parent_idx]);

        this->trickle_up(parent_idx);
    }
}

template <typename ValueType, typename LessThan>
void MutablePriorityQueue<ValueType, LessThan>::trickle_down(size_t node_idx) {
    size_t left_child_idx = (node_idx << 1) + 1;
    size_t right_child_idx = (node_idx << 1) + 2;

    size_t largest_node_idx = node_idx;

    if (left_child_idx < heap_.size() &&
        LessThan(heap_[largest_node_idx]->value, heap_[left_child_idx]->value)) {
        largest_node_idx = left_child_idx;
    }

    if (right_child_idx < heap_.size() &&
        LessThan(heap_[largest_node_idx]->value, heap_[right_child_idx]->value)) {
        largest_node_idx = right_child_idx;
    }

    if (largest_node_idx != node_idx) {
        std::swap(heap_[node_idx]->idx, heap_[largest_node_idx]->idx);
        std::swap(heap_[node_idx], heap_[largest_node_idx]);
        std::swap(node_idx, largest_node_idx);

        this->trickle_down(node_idx);
    }
}

template <typename ValueType, typename LessThan>
template <typename HeapType, typename IteratorValueType>
MutablePriorityQueue<ValueType, LessThan>::iterator_base<HeapType, IteratorValueType>::
    iterator_base(HeapType *heap,
                  typename MutablePriorityQueue<ValueType, LessThan>::Node *current_node)
    : heap_(heap), current_node_(current_node) {}

template <typename ValueType, typename LessThan>
template <typename HeapType, typename IteratorValueType>
bool MutablePriorityQueue<ValueType, LessThan>::iterator_base<
    HeapType, IteratorValueType>::operator==(iterator_base const &rhs) const {
    return current_node_ == rhs.current_node_;
}

template <typename ValueType, typename LessThan>
template <typename HeapType, typename IteratorValueType>
bool MutablePriorityQueue<ValueType, LessThan>::iterator_base<
    HeapType, IteratorValueType>::operator!=(iterator_base const &rhs) const {
    return !(*this == rhs);
}

template <typename ValueType, typename LessThan>
template <typename HeapType, typename IteratorValueType>
typename MutablePriorityQueue<ValueType, LessThan>::template iterator_base<HeapType,
                                                                           IteratorValueType> &
MutablePriorityQueue<ValueType, LessThan>::iterator_base<HeapType,
                                                         IteratorValueType>::operator++() {
    assert(current_node_ != nullptr);
    size_t next_idx = current_node_->idx + 1;

    if (next_idx < heap_->size()) {
        current_node_ = heap_[next_idx].get();
    } else {
        current_node_ = nullptr;
    }

    return *this;
}

template <typename ValueType, typename LessThan>
template <typename HeapType, typename IteratorValueType>
IteratorValueType &
MutablePriorityQueue<ValueType, LessThan>::iterator_base<HeapType, IteratorValueType>::operator*() {
    assert(current_node_ != nullptr);
    return current_node_->value;
}

} // namespace e8

#endif // FIXEDPRIORITYQUEUE_H