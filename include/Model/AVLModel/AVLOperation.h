#pragma once
#include <vector>
enum class AVLOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search };

struct AVLOperation {
    AVLOperationType type;

    int value;

    static AVLOperation insert(const int& value) {
        AVLOperation op;
        op.type = AVLOperationType::Insert;
        op.value = value;
        return op;
    }

    static AVLOperation remove(const int& key) {
        AVLOperation op;
        op.type = AVLOperationType::Remove;
        op.value = key;
        return op;
    }

    static AVLOperation search(const int& key) {
        AVLOperation op;
        op.type = AVLOperationType::Search;
        op.value = key;
        return op;
    }

    static AVLOperation update(const int& value) {
        AVLOperation op;
        op.type = AVLOperationType::Update;
        op.value = value;
        return op;
    }

    static AVLOperation reset() {
        AVLOperation op;
        op.type = AVLOperationType::Reset;
        return op;
    }
};