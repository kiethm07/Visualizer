#pragma once
#include <Model/LinkedListModel/LinkedListState.h>
#include <Model/LinkedListModel/ListOperation.h>

class LinkedList {
public:
    void insert(int x, int k) { insert(pHead, x, k); }
    void insertFirst(int x) { insertFirst(pHead, x); }
    void remove(int x) { remove(pHead, x); }
    void update(int x, int k) { update(pHead, x, k); }
    void clear() { clear(pHead); }
    LinkedList();
    ~LinkedList();
    LinkedListState getState();
    void loadState(const LinkedListState& state);
    void applyOperation(const ListOperation& operation);
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
    void insert(Node*& pHead, const int& x, const int& k);
    void clear(Node*& pHead);
    void insertFirst(Node*& pHead, int x);
    void remove(Node*& pHead, int k);
    void removeFirst(Node*& pHead);
    void update(Node*& pHead, int x, int k);
};