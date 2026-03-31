#pragma once
#include <vector>
enum class ListOperationType {InsertSingle, InsertMultiple, Remove, Update, Reset, Search};

struct ListOperation {
    ListOperationType type;

    int position = -1;
    int value = 0;
    std::vector<int> values;

    static ListOperation insertSingle(int position, int value) {
        ListOperation op;
        op.type = ListOperationType::InsertSingle;
        op.position = position;
        op.value = value;
        return op;
    }

    static ListOperation insertMultiple(int position, const std::vector<int>& values) {
        ListOperation op;
        op.type = ListOperationType::InsertMultiple;
        op.position = position;
        op.values = values;
        return op;
    }

    static ListOperation remove(int position) {
        ListOperation op;
        op.type = ListOperationType::Remove;
        op.position = position;
        return op;
    }

    static ListOperation search(int value) {
        ListOperation op;
        op.type = ListOperationType::Search;
        op.value = value;
        return op;
    }

    static ListOperation update(int position, int value) {
        ListOperation op;
        op.type = ListOperationType::Update;
        op.position = position;
        op.value = value;
        return op;
    }

    static ListOperation reset() {
        ListOperation op;
        op.type = ListOperationType::Reset;
        return op;
    }
};