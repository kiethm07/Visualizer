#include <DataStructures/Trie.h>

Trie::Trie() :
	next_ui_id(0), root(nullptr){
	root = new Node("root", next_ui_id++);
}

void Trie::clearWithoutRecorder(Node*& root) {
	if (root == nullptr) return;
	for (int i = 0; i < 26; i++) {
		clearWithoutRecorder(root->child[i]);
	}
	delete root;
	root = nullptr;
}

Trie::~Trie() {
	clearWithoutRecorder(root);
}

void Trie::applyOperation(const TrieOperation& operation, TrieRecorder& recorder){
	using namespace std;
	if (operation.type == TrieOperationType::Insert) {
		string x = operation.value;
		insert(x, recorder);
		return;
	}
	if (operation.type == TrieOperationType::Remove) {
		string x = operation.value;
		remove(x, recorder);
		return;
	}
	if (operation.type == TrieOperationType::Search) {
		string x = operation.value;
		search(x, recorder);
		return;
	}
	if (operation.type == TrieOperationType::Reset) {
		clear(root, recorder);
		return;
	}
}

void Trie::rawInit(const std::vector<std::string>& values) {
	for (const std::string& s : values) {
		Node* tmp = root;
		for (int i = 0; i < s.size(); i++) {
			int nxt = s[i] - 'A';
			if (tmp->child[nxt] == nullptr) {
				std::string label = "";
				label += s[i];
				tmp->child[nxt] = new Node(label, next_ui_id++);
			}
			tmp = tmp->child[nxt];
			tmp->cnt++;
		}
		tmp->isEnd = 1;
	}
}

void Trie::insert(const std::string& s, TrieRecorder& recorder) {
	Node* tmp = root;
	for (int i = 0; i < s.size(); i++) {
		int nxt = s[i] - 'A';
		if (tmp->child[nxt] == nullptr) {
			std::string label = "";
			label += s[i];
			tmp->child[nxt] = new Node(label, next_ui_id++);
		}
		tmp = tmp->child[nxt];
		tmp->cnt++;
	}
	tmp->isEnd = 1;
}

void Trie::removeBranch(Node*& root, const std::string& s, int idx, TrieRecorder& recorder) {
	if (root == nullptr || idx == s.size()) return;
	root->cnt--;
	if (idx + 1 < s.size()) {
		removeBranch(root->child[s[idx + 1] - 'A'], s, idx + 1, recorder);
	}
	if (idx + 1 == s.size()) {
		root->isEnd = 0;
	}
	if (root->cnt == 0) {
		delete root;
		root = nullptr;
	}
}

void Trie::remove(const std::string& s, TrieRecorder& recorder) {
	//Validate query
	//for (int i = 0; i < s.size(); i++) {
	//	int nxt = s[i] - 'a';
	//	if (tmp->child[nxt] == nullptr) {
	//		//Not found!
	//		return;
	//	}
	//	tmp = tmp->child[nxt];
	//}
	if (0 < s.size()) removeBranch(root->child[s[0] - 'A'], s, 0, recorder);
}

void Trie::clear(Node*& root, TrieRecorder& recorder) {
	if (root == nullptr) return;
	for (int i = 0; i < 26; i++) {
		clear(root->child[i], recorder);
	}
	delete root;
	root = nullptr;
}

void Trie::search(const std::string& s, TrieRecorder& recorder) {
	Node* tmp = root;
	for (int i = 0; i < s.size(); i++) {
		int nxt = s[i] - 'A';
		if (tmp->child[nxt] == nullptr) {
			return;
		}
		tmp = tmp->child[nxt];
	}
}