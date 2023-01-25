#include "catch.hpp"

#include "../src/BinaryTrie.h"

TEST_CASE("[BinaryTrie]: Go left right") {
    BinaryTrie<int> trie;
    auto root = trie.Root();
    root->value = 0;
    auto left = trie.Go(root, false);
    left->value = 1;
    auto right = trie.Go(root, true);
    right->value = 2;
    REQUIRE(trie.Go(root, false)->value == 1);
    REQUIRE(trie.Go(root, true)->value == 2);
}

TEST_CASE("[BinaryTrie]: Add path") {
    BinaryTrie<int> trie;
    auto v1 = trie.AddPath(0b001, 3);
    v1->value = 1;
    REQUIRE(trie.Right(trie.Left(trie.Left(trie.Root())))->value == 1);

    auto v2 = trie.AddPath(0b011, 3);
    v2->value = 2;
    REQUIRE(trie.Right(trie.Right(trie.Left(trie.Root())))->value == 2);

    auto v3 = trie.AddPath(0b011, 3);
    REQUIRE(v3->value == 2);

    auto v4 = trie.AddPath(0b0110, 4);
    v4->value = 4;
    REQUIRE(trie.Left(trie.Right(trie.Right(trie.Left(trie.Root()))))->value == 4);
}
