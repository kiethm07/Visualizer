#pragma once
#include <vector>
#include <string>
enum class ListOperationType {InsertSingle, InsertMultiple, Remove, Update, Reset, Search, Home, Setting, Save, Load};

struct ListOperation {
    ListOperationType type;
    std::string file_path = "";
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

    static ListOperation save(const std::string& s) {
        ListOperation op;
        op.type = ListOperationType::Save;
        op.file_path = s;
        return op;
    }

    static ListOperation load(const std::string& s) {
        ListOperation op;
        op.type = ListOperationType::Load;
        op.file_path = s;
        return op;
    }

    static ListOperation home() {
        ListOperation op;
        op.type = ListOperationType::Home;
        return op;
    }

    static ListOperation setting() {
        ListOperation op;
        op.type = ListOperationType::Setting;
        return op;
    }
};