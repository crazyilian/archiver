#pragma once

#include <cstdint>

template <typename Value>
class BinaryTrie {
public:
    class Node {
    public:
        Value value;
        Node *left;
        Node *right;

        Node();
        explicit Node(const Value &value);
        ~Node();
    };

    BinaryTrie();
    ~BinaryTrie();

    Node *Root();
    Node *Left(Node *node);
    Node *Right(Node *node);
    Node *Go(Node *node, bool right);
    Node *AddPath(uint64_t path, uint8_t length);

private:
    Node *root_;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// Definitions //////////////////////////////////////////

template <typename Value>
BinaryTrie<Value>::Node::Node() : left(nullptr), right(nullptr) {
}

template <typename Value>
BinaryTrie<Value>::Node::Node(const Value &value) : value(value), left(nullptr), right(nullptr) {
}

template <typename Value>
BinaryTrie<Value>::Node::~Node() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}

template <typename Value>
BinaryTrie<Value>::BinaryTrie() {
    root_ = new Node();
}

template <typename Value>
BinaryTrie<Value>::~BinaryTrie() {
    delete root_;
};

template <typename Value>
typename BinaryTrie<Value>::Node *BinaryTrie<Value>::Root() {
    return root_;
}

template <typename Value>
typename BinaryTrie<Value>::Node *BinaryTrie<Value>::Left(BinaryTrie::Node *node) {
    if (!node->left) {
        node->left = new Node();
    }
    return node->left;
}

template <typename Value>
typename BinaryTrie<Value>::Node *BinaryTrie<Value>::Right(BinaryTrie::Node *node) {
    if (!node->right) {
        node->right = new Node();
    }
    return node->right;
}

template <typename Value>
typename BinaryTrie<Value>::Node *BinaryTrie<Value>::Go(BinaryTrie::Node *node, bool right) {
    if (right) {
        return Right(node);
    } else {
        return Left(node);
    }
}

template <typename Value>
typename BinaryTrie<Value>::Node *BinaryTrie<Value>::AddPath(uint64_t path, uint8_t length) {
    auto node = root_;
    for (uint8_t index = 0; index < length; ++index) {
        bool bit = (path >> (length - index - 1)) & 1;
        node = Go(node, bit);
    }
    return node;
}