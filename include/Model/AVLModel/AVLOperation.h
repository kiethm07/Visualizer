#pragma once
#include <vector>
#include <string>
enum class AVLOperationType { Insert, InsertMultiple, Remove, Update, Reset, Save, Load, Search, Home, Setting };

struct AVLOperation {
    AVLOperationType type;
    std::string file_path = "";
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

    static AVLOperation save(const std::string& s) {
        AVLOperation op;
        op.type = AVLOperationType::Save;
        op.file_path = s;
        return op;
    }

    static AVLOperation load(const std::string& s) {
        AVLOperation op;
        op.type = AVLOperationType::Load;
        op.file_path = s;
        return op;
    }

    static AVLOperation home() {
        AVLOperation op;
        op.type = AVLOperationType::Home;
        return op;
    }

    static AVLOperation setting() {
        AVLOperation op;
        op.type = AVLOperationType::Setting;
        return op;
    }
};