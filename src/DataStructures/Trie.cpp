#include <DataStructures/Trie.h>
#include <map>

Trie::Trie() :
	next_ui_id(0), root(nullptr){
	root = new Node("root", next_ui_id++);
}

Trie::~Trie() {
	clearWithoutRecorder(root);
}

TrieState Trie::getState() const {
	TrieState state;
	if (!root) return state;
	std::vector<Node*> all_nodes;
	std::map<Node*, int> node_to_idx;
	auto collect = [&](auto self, Node* u) -> void {
		node_to_idx[u] = (int)all_nodes.size();
		all_nodes.push_back(u);
		for (int i = 0; i < 26; i++) {
			if (u->child[i]) self(self, u->child[i]);
		}
		};
	collect(collect, root);
	for (Node* u : all_nodes) {
		TrieNodeSnapshot snp;
		snp.label = u->label;
		snp.ui_id = u->ui_id;
		snp.cnt = u->cnt;
		snp.isEnd = u->isEnd;
		for (int i = 0; i < 26; i++) {
			snp.child[i] = u->child[i] ? node_to_idx[u->child[i]] : -1;
		}
		state.nodes.push_back(snp);
	}
	state.next_ui_id = this->next_ui_id;
	return state;
}

void Trie::loadState(const TrieState& state) {
	if (state.nodes.empty()) {
		root = nullptr;
		return;
	}
	int n = (int)state.nodes.size();
	std::vector<Node*> new_nodes(n);
	for (int i = 0; i < n; i++) {
		const auto& snp = state.nodes[i];
		new_nodes[i] = new Node(snp.label, snp.ui_id);
		new_nodes[i]->cnt = snp.cnt;
		new_nodes[i]->isEnd = snp.isEnd;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 26; j++) {
			int child_idx = state.nodes[i].child[j];
			if (child_idx != -1 && child_idx < n) {
				new_nodes[i]->child[j] = new_nodes[child_idx];
			}
		}
	}
	root = new_nodes[0];
	this->next_ui_id = state.next_ui_id;
}

void Trie::clearWithoutRecorder(Node*& root) {
	if (root == nullptr) return;
	for (int i = 0; i < 26; i++) {
		clearWithoutRecorder(root->child[i]);
	}
	delete root;
	root = nullptr;
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