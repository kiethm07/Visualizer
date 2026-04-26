#pragma once
#include <vector>
#include <string>
enum class TrieOperationType { Insert, InsertMultiple, Remove, Update, Reset, Search, Setting, Home, Save, Load };

struct TrieOperation {
    TrieOperationType type;
    std::string file_path;
    std::string value;

    static TrieOperation insert(const std::string& value) {
        TrieOperation op;
        op.type = TrieOperationType::Insert;
        op.value = value;
        return op;
    }

    static TrieOperation remove(const std::string& key) {
        TrieOperation op;
        op.type = TrieOperationType::Remove;
        op.value = key;
        return op;
    }

    static TrieOperation search(const std::string& key) {
        TrieOperation op;
        op.type = TrieOperationType::Search;
        op.value = key;
        return op;
    }

    static TrieOperation update(const std::string& value) {
        TrieOperation op;
        op.type = TrieOperationType::Update;
        op.value = value;
        return op;
    }

    static TrieOperation reset() {
        TrieOperation op;
        op.type = TrieOperationType::Reset;
        return op;
    }

    static TrieOperation save(const std::string& s) {
        TrieOperation op;
        op.type = TrieOperationType::Save;
        op.file_path = s;
        return op;
    }

    static TrieOperation load(const std::string& s) {
        TrieOperation op;
        op.type = TrieOperationType::Load;
        op.file_path = s;
        return op;
    }

    static TrieOperation home() {
        TrieOperation op;
        op.type = TrieOperationType::Home;
        return op;
    }

    static TrieOperation setting() {
        TrieOperation op;
        op.type = TrieOperationType::Setting;
        return op;
    }

};