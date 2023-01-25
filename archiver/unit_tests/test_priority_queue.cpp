#include "catch.hpp"

#include "../src/BinaryHeap.h"

template <typename T>
void CheckHeapSort(std::vector<T> values) {
    BinaryHeap<int> heap;
    REQUIRE(heap.size() == 0);
    for (size_t i = 0; i < values.size(); ++i) {
        heap.Insert(values[i]);
        REQUIRE(heap.size() == i + 1);
    }
    std::vector<T> sorted;
    while (heap.size() > 0) {
        auto x = heap.GetMin();
        REQUIRE(x == heap.PopMin());
        sorted.push_back(x);
    }
    std::sort(values.begin(), values.end());
    REQUIRE(values == sorted);
}

TEST_CASE("[PriorityQueue]: Heap sort") {
    CheckHeapSort(std::vector<int>{1, 1, 1});
    CheckHeapSort(std::vector<int>{1, 2, 3, 4, 5});
    CheckHeapSort(std::vector<int>{5, 4, 3, 2, 1});
    CheckHeapSort(std::vector<int>{1, 1, -1, -1, -1, 1, -1});
}
