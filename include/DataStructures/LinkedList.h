#pragma once
#include <Model/LinkedListModel/LinkedListState.h>

class LinkedList {
public:
    void insertFirst(int x) { insertFirst(pHead, x); }
    void remove(int x);
    void clear() { clear(pHead); }
    LinkedList();
    ~LinkedList();
    LinkedListState getState();
    void loadState(const LinkedListState& state);

    ///Debug purpose
    void print();
private:
    struct Node {
        int val;
        int ui_id; //Specific for UI
        Node* pNext;
        Node() :
            val(0), ui_id(-1), pNext(nullptr) {
        }
        Node(int x, int ui_id) :
            val(x), ui_id(ui_id), pNext(nullptr) {
        }
    };
    int next_ui_id;
    Node* pHead;
    void clear(Node*& pHead);
    void insertFirst(Node*& pHead, int x);
    void remove(Node*& pHead, int x);
};