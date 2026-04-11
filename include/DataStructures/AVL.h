#pragma once

class AVL {
private:
    struct Node {
        int val, height, size;
        Node* pLeft, * pRight;
        Node() {}
        Node(int x) :
            val(x), height(1), size(1), pLeft(nullptr), pRight(nullptr) {
        }
    };
    Node* root;
    int size(Node*& root) {
        if (root == nullptr) return 0;
        return root->size;
    }
    void clear(Node*& root) {
        if (root == nullptr) return;
        clear(root->pLeft);
        clear(root->pRight);
        delete root;
        root = nullptr;
    }
    void updateState(Node*& u) {
        if (u == nullptr) return;
        u->height = max(getHeight(u->pLeft), getHeight(u->pRight)) + 1;
        u->size = size(u->pLeft) + size(u->pRight) + 1;
    }
    void rotateLeft(Node*& u) {
        if (u == nullptr) return;
        Node* tmp = u->pRight;
        if (tmp == nullptr) return;
        u->pRight = tmp->pLeft;
        tmp->pLeft = u;
        updateState(u);
        updateState(tmp);
        u = tmp;
    }
    void rotateRight(Node*& u) {
        if (u == nullptr) return;
        Node* tmp = u->pLeft;
        if (tmp == nullptr) return;
        u->pLeft = tmp->pRight;
        tmp->pRight = u;
        updateState(u);
        updateState(tmp);
        u = tmp;
    }
    void insert(Node*& root, int x) {
        if (root == nullptr) {
            root = new Node(x);
            return;
        }
        if (root->val == x) {
            return;
        }
        if (root->val < x) insert(root->pRight, x);
        if (root->val > x) insert(root->pLeft, x);
        updateState(root);
        balance(root);
    }
    void balance(Node*& u) {
        if (u == nullptr) return;
        int difference = getHeight(u->pLeft) - getHeight(u->pRight);
        if (difference >= -1 && difference <= 1) return;
        if (difference > 1) {
            Node*& t = u->pLeft;
            if (t && getHeight(t->pLeft) < getHeight(t->pRight)) {
                rotateLeft(t);
            }
            rotateRight(u);
            return;
        }
        Node*& t = u->pRight;
        if (t && getHeight(t->pRight) < getHeight(t->pLeft)) {
            rotateRight(t);
        }
        rotateLeft(u);
        return;
    }
    void erase(Node*& root, int x) {
        if (root == nullptr) return;
        if (root->val == x) {
            if (root->pRight == nullptr && root->pLeft == nullptr) {
                delete root;
                root = nullptr;
                return;
            }
            if (root->pRight == nullptr) {
                Node* tmp = root->pLeft;
                delete root;
                root = tmp;
                return;
            }
            if (root->pLeft == nullptr) {
                Node* tmp = root->pRight;
                delete root;
                root = tmp;
                return;
            }
            Node* tmp = root->pRight;
            while (tmp->pLeft != nullptr) tmp = tmp->pLeft;
            root->val = tmp->val;
            erase(root->pRight, tmp->val);
            updateState(root);
            balance(root);
            return;
        }
        if (root->val < x) erase(root->pRight, x);
        if (root->val > x) erase(root->pLeft, x);
        updateState(root);
        balance(root);
    }
    void inOrder(Node*& root) {
        if (root == nullptr) return;
        cout << root->val << " ";
        inOrder(root->pLeft);
        inOrder(root->pRight);
    }
    void preOrder(Node*& root) {
        if (root == nullptr) return;
        preOrder(root->pLeft);
        cout << root->val << " ";
        preOrder(root->pRight);
    }
    int getHeight(Node*& root) {
        if (root == nullptr) return 0;
        return root->height;
    }
    bool search(Node*& root, int x) {
        if (root == nullptr) return false;
        if (root->val == x) return true;
        if (root->val < x) return search(root->pRight, x);
        return search(root->pLeft, x);
    }
public:
    AVL() :
        root(nullptr) {
    }
    ~AVL() {
        clear(root);
    }
    void clear() { clear(root); }
    void insert(int x) { insert(root, x); }
    void erase(int x) { erase(root, x); }
    void printInOrder() { inOrder(root); }
    void printPreOrder() { preOrder(root); }
    void rotateLeft() { rotateLeft(root); }
    void rotateRight() { rotateRight(root); }
    int height() { return getHeight(root); }
    bool search(int x) { return search(root, x); }
};