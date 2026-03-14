#include <DataStructures/LinkedList.h>
#include <iostream>

LinkedListState LinkedList::getState() {
    LinkedListState state;
    state.next_ui_id = this->next_ui_id;
    for (LinkedList::Node* cur = pHead; cur != nullptr; cur = cur->pNext){
        state.value.push_back(cur->val);
        state.ui_id.push_back(cur->ui_id);
    }
    return state;
}

void LinkedList::loadState(const LinkedListState& state) {
    this->next_ui_id = state.next_ui_id;
    clear(pHead);
    Node* cur = nullptr;
    for (int i = 0; i < state.value.size(); i++) {
        int val = state.value[i];
        int ui_id = state.ui_id[i];
        if (i == 0) {
            pHead = new Node(val, ui_id);
            cur = pHead;
        }
        else {
            cur->pNext = new Node(val, ui_id);
            cur = cur->pNext;
        }
    }
}

void LinkedList::applyOperation(const ListOperation& operation) {
    if (operation.type == ListOperationType::InsertSingle) {
        int x = operation.value;
        int k = operation.position;
        insert(pHead, x, k);
        return;
    }
    if (operation.type == ListOperationType::InsertMultiple) {

        return;
    }
    if (operation.type == ListOperationType::Delete) {
        int k = operation.position;
        return;
    }
}

LinkedList::LinkedList() :
    pHead(nullptr), next_ui_id(0) {
}

void LinkedList::clear(LinkedList::Node*& pHead) {
    if (pHead == nullptr) return;
    clear(pHead->pNext);
    delete pHead;
    pHead = nullptr;
}

LinkedList::~LinkedList() {
    clear(pHead);
}

void LinkedList::insert(LinkedList::Node*& pHead, const int& x, const int& k) {
    if (k < 0) return;
    if (k == 0) {
        insertFirst(pHead, x);
        return;
    }
    Node* cur = pHead;
    int i = 0;
    for (i; i < k - 1 && cur != nullptr; i++) {
        cur = cur->pNext;
    }
    if (i < k - 1) return;
    if (cur == nullptr) return;
    Node* tmp = new Node(x, ++next_ui_id);
    tmp->pNext = cur->pNext;
    cur->pNext = tmp;
}

void LinkedList::insertFirst(LinkedList::Node*& pHead, int x) {
    Node* tmp = new Node(x, ++next_ui_id);
    tmp->pNext = pHead;
    pHead = tmp;
}

void LinkedList::print() {
    Node* cur = pHead;
    while (cur != nullptr) {
        std::cout << cur->val << " ";
        cur = cur->pNext;
    }
    return;
}