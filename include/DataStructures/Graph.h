#pragma once
#include <Model/GraphModel/GraphState.h>
#include <Model/GraphModel/GraphOperation.h>
#include <Animation/Graph/GraphRecorder.h>
#include <vector>
#include <string>
#include <map>

class Graph {
public:
	Graph();
	~Graph();
	GraphState getState() const;
	void loadState(const GraphState& state);
	void applyOperation(const GraphOperation& operation, GraphRecorder& recorder);
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
	void rotateLeft(Node*& u, GraphRecorder& recorder);
	void rotateRight(Node*& u, GraphRecorder& recorder);
	void balance(Node*& u, GraphRecorder& recorder);
	void insert(Node*& root, int x, GraphRecorder& recorder);
	void remove(Node*& root, int x, GraphRecorder& recorder);
	void clear(Node*& root, GraphRecorder& recorder);
	bool search(Node* root, int x, GraphRecorder& recorder);
	void clearWithoutRecorder(Node*& root);
};