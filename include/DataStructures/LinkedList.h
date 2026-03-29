#pragma once
#include <Model/LinkedListModel/LinkedListState.h>
#include <Model/LinkedListModel/ListOperation.h>
#include <Animation/LinkedList/LinkedListRecorder.h>

class LinkedList {
public:
    //void insert(int x, int k) { insert(pHead, x, k); }
    //void insertFirst(int x) { insertFirst(pHead, x); }
    //void remove(int x) { remove(pHead, x); }
    //void update(int x, int k) { update(pHead, x, k); }
    //void clear() { clear(pHead); }
    LinkedList();
    ~LinkedList();
    LinkedListState getState();
    void loadState(const LinkedListState& state);
    void applyOperation(const ListOperation& operation, LinkedListRecorder& recorder);
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
    void insert(Node*& pHead, const int& x, const int& k, LinkedListRecorder& recorder);
    void update(Node*& pHead, const int& x, const int& k, LinkedListRecorder& recorder);
    void clear(Node*& pHead, LinkedListRecorder& recorder);
    void insertFirst(Node*& pHead, int x, LinkedListRecorder& recorder);
    void remove(Node*& pHead, int k, LinkedListRecorder& recorder);
    void removeFirst(Node*& pHead, LinkedListRecorder& recorder);
	void clearWithoutRecorder(Node*& pHead);
};