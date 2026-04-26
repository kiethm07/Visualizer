#pragma once
#include <vector>
#include <string>
enum class HashmapOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search, Save, Load, Home, Setting };

struct HashmapOperation {
    HashmapOperationType type;
    std::string file_path = "";
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
        op.value = key;
        return op;
    }

    static HashmapOperation search(int key) {
        HashmapOperation op;
        op.type = HashmapOperationType::Search;
        op.key = key;
        op.value = key;
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

    static HashmapOperation save(const std::string& s) {
        HashmapOperation op;
        op.type = HashmapOperationType::Save;
        op.file_path = s;
        return op;
    }

    static HashmapOperation load(const std::string& s) {
        HashmapOperation op;
        op.type = HashmapOperationType::Load;
        op.file_path = s;
        return op;
    }

    static HashmapOperation home() {
        HashmapOperation op;
        op.type = HashmapOperationType::Home;
        return op;
    }

    static HashmapOperation setting() {
        HashmapOperation op;
        op.type = HashmapOperationType::Setting;
        return op;
    }
};