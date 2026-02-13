#include <DataStructures/LinkedList.h>
#include <iostream>

LinkedList::LinkedList() :
    pHead(nullptr) {
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

void LinkedList::insertFirst(LinkedList::Node*& pHead, int x) {
    Node* tmp = new Node(x);
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