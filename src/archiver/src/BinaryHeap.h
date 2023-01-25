#pragma once

#include <stddef.h>

#include <vector>

template <typename T, typename Container = std::vector<T>, typename Compare = std::less<typename Container::value_type>>
class BinaryHeap {
public:
    BinaryHeap();

    const T &GetMin() const;
    T PopMin();
    void Insert(const T &value);
    size_t size() const;  // NOLINT

private:
    Container heap_;

    void ShiftDownRoot();
    void ShiftUp(size_t node);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Definitions //////////////////////////////////////////

template <typename T, typename Container, typename Compare>
BinaryHeap<T, Container, Compare>::BinaryHeap() {
}

template <typename T, typename Container, typename Compare>
const T &BinaryHeap<T, Container, Compare>::GetMin() const {
    return heap_[0];
}

template <typename T, typename Container, typename Compare>
T BinaryHeap<T, Container, Compare>::PopMin() {
    T min = heap_[0];
    std::swap(heap_[0], heap_.back());
    heap_.pop_back();
    ShiftDownRoot();
    return min;
}

template <typename T, typename Container, typename Compare>
void BinaryHeap<T, Container, Compare>::Insert(const T &value) {
    heap_.push_back(value);
    ShiftUp(heap_.size() - 1);
}

template <typename T, typename Container, typename Compare>
void BinaryHeap<T, Container, Compare>::ShiftDownRoot() {
    size_t node = 0;
    while (2 * node + 1 < heap_.size()) {
        auto left = 2 * node + 1;
        auto right = 2 * node + 2;
        auto min_child = left;
        if (right < heap_.size() && Compare()(heap_[right], heap_[left])) {
            min_child = right;
        }
        if (!Compare()(heap_[min_child], heap_[node])) {
            break;
        }
        std::swap(heap_[node], heap_[min_child]);
        node = min_child;
    }
}

template <typename T, typename Container, typename Compare>
void BinaryHeap<T, Container, Compare>::ShiftUp(size_t node) {
    while (node > 0 && heap_[node] < heap_[(node - 1) / 2]) {
        std::swap(heap_[node], heap_[(node - 1) / 2]);
        node = (node - 1) / 2;
    }
}

template <typename T, typename Container, typename Compare>
size_t BinaryHeap<T, Container, Compare>::size() const {
    return heap_.size();
}
