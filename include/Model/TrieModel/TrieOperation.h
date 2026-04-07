#pragma once
#include <vector>
enum class TrieOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search };

struct TrieOperation {
    TrieOperationType type;

    int key = 0;
    int value = 0;
    std::vector<int> keys;
    std::vector<int> values;

    static TrieOperation insert(int key, int value) {
        TrieOperation op;
        op.type = TrieOperationType::Insert;
        op.key = key;
        op.value = value;
        return op;
    }

    static TrieOperation insertMultiple(const std::vector<int>& keys, const std::vector<int>& values) {
        TrieOperation op;
        op.type = TrieOperationType::InsertMultiple;
        op.keys = keys;
        op.values = values;
        return op;
    }

    static TrieOperation remove(int key) {
        TrieOperation op;
        op.type = TrieOperationType::Remove;
        op.key = key;
        op.value = key;
        return op;
    }

    static TrieOperation search(int key) {
        TrieOperation op;
        op.type = TrieOperationType::Search;
        op.key = key;
        op.value = key;
        return op;
    }

    static TrieOperation update(int key, int value) {
        TrieOperation op;
        op.type = TrieOperationType::Update;
        op.key = key;
        op.value = value;
        return op;
    }

    static TrieOperation reset() {
        TrieOperation op;
        op.type = TrieOperationType::Reset;
        return op;
    }
};