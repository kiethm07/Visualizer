#pragma once

class LinkedList {
public:
    void insertFirst(int x) { insertFirst(pHead, x); }
    void remove(int x);
    LinkedList();
    ~LinkedList();

    ///Debug purpose
    void print();
private:
    struct Node {
        int val;
        Node* pNext;
        Node() {}
        Node(int x) :
            val(x), pNext(nullptr) {
        }
    };
    Node* pHead;
    void clear(Node*& pHead);
    void insertFirst(Node*& pHead, int x);
    void remove(Node*& pHead, int x);
};