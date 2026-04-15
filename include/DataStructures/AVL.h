#pragma once
#include <Model/AVLModel/AVLState.h>
#include <Model/AVLModel/AVLOperation.h>
#include <Animation/AVL/AVLRecorder.h>
#include <vector>
#include <string>
#include <map>

class AVL {
public:
	AVL();
	~AVL();
	AVLState getState() const;
	void loadState(const AVLState& state);
	void applyOperation(const AVLOperation& operation, AVLRecorder& recorder);
	void rawInit(const std::vector<int>& values);
private:
	struct Node {
		int val, height, size, ui_id;
		Node* pLeft, * pRight, * pParent;
		Node(int x, int id) : val(x), height(1), size(1), ui_id(id), pLeft(nullptr), pRight(nullptr) {}
	};
	Node* root = nullptr;
	int next_ui_id = 0;
	int getHeight(Node* u);
	int getSize(Node* u);
	void updateState(Node*& u);
	void rotateLeft(Node*& u, AVLRecorder& recorder);
	void rotateRight(Node*& u, AVLRecorder& recorder);
	void balance(Node*& u, AVLRecorder& recorder);
	void insert(Node*& root, int x, AVLRecorder& recorder);
	void remove(Node*& root, int x, AVLRecorder& recorder);
	void clear(Node*& root, AVLRecorder& recorder);
	bool search(Node* root, int x, AVLRecorder& recorder);
	void clearWithoutRecorder(Node*& root);
};