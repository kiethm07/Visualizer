#pragma once
#include <vector>
enum class HashmapOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search };

struct HashmapOperation {
    HashmapOperationType type;

    int key = 0;
    int value = 0;
    std::vector<int> keys;
    std::vector<int> values;

    static HashmapOperation insert(int key, int value) {
        HashmapOperation op;
        op.type = HashmapOperationType::Insert;
        op.key = key;
        op.value = value;
        return op;
    }

    static HashmapOperation insertMultiple(const std::vector<int>& keys, const std::vector<int>& values) {
        HashmapOperation op;
        op.type = HashmapOperationType::InsertMultiple;
        op.keys = keys;
        op.values = values;
        return op;
    }

    static HashmapOperation remove(int key) {
        HashmapOperation op;
        op.type = HashmapOperationType::Remove;
        op.key = key;
        return op;
    }

    static HashmapOperation search(int key) {
        HashmapOperation op;
        op.type = HashmapOperationType::Search;
        op.key = key;
        return op;
    }

    static HashmapOperation update(int key, int value) {
        HashmapOperation op;
        op.type = HashmapOperationType::Update;
        op.key = key;
        op.value = value;
        return op;
    }

    static HashmapOperation reset() {
        HashmapOperation op;
        op.type = HashmapOperationType::Reset;
        return op;
    }
};