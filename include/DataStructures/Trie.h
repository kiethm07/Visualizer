#pragma once
#include <Model/TrieModel/TrieState.h>
#include <Model/TrieModel/TrieOperation.h>
#include <Animation/Trie/TrieRecorder.h>
#include <vector>

class Trie {
public:
	Trie();
	~Trie();
	TrieState getState() const;
	void init(int n);
	void loadState(const TrieState& state);
	void applyOperation(const TrieOperation& operation, TrieRecorder& recorder);
	void rawInit(const int& bucket_count, const std::vector<int>& values);
private:
	struct Node {
		std::string label;
		int ui_id;
		Node* child[26];
		int cnt = 0;
		bool isEnd = 0;
		Node() {
			for (int i = 0; i < 26; i++) child[i] = nullptr;
		}
		Node(const std::string& label, int ui_id):
			label(label), ui_id(ui_id) {
			for (int i = 0; i < 26; i++) child[i] = nullptr;
			isEnd = 0;
		}
	};
	Node* root = nullptr;
	int next_ui_id = 0;
	void insert(const std::string& s, TrieRecorder& recorder);
	void remove(const std::string& s, TrieRecorder& recorder);
	void clear(Node*& root, TrieRecorder& recorder);
	void search(const std::string& s, TrieRecorder& recorder);
	void clearWithoutRecorder(Node*& root);
	void removeBranch(Node*& root, const std::string& s, int idx, TrieRecorder& recorder);
};