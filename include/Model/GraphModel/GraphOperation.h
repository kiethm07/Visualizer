#pragma once
#include <vector>
enum class GraphOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search };

struct GraphOperation {
    GraphOperationType type;

    int value;

    static GraphOperation insert(const int& value) {
        GraphOperation op;
        op.type = GraphOperationType::Insert;
        op.value = value;
        return op;
    }

    static GraphOperation remove(const int& key) {
        GraphOperation op;
        op.type = GraphOperationType::Remove;
        op.value = key;
        return op;
    }

    static GraphOperation search(const int& key) {
        GraphOperation op;
        op.type = GraphOperationType::Search;
        op.value = key;
        return op;
    }

    static GraphOperation update(const int& value) {
        GraphOperation op;
        op.type = GraphOperationType::Update;
        op.value = value;
        return op;
    }

    static GraphOperation reset() {
        GraphOperation op;
        op.type = GraphOperationType::Reset;
        return op;
    }
};