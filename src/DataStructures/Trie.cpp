#include <DataStructures/Trie.h>
#include <map>

Trie::Trie() :
	next_ui_id(0), root(nullptr) {
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

void Trie::saveToFile(const std::string& filepath) const {
	if (filepath.empty()) return;

	TrieState state = getState();

	std::ofstream fout(filepath);
	if (!fout.is_open()) {
		std::cerr << "Cannot open file: " << filepath << "\n";
		return;
	}

	fout << state.next_ui_id << " " << state.nodes.size() << "\n";

	for (const auto& snp : state.nodes) {
		fout << (snp.label.empty() ? "-" : snp.label) << " "
			<< snp.ui_id << " "
			<< snp.cnt << " "
			<< snp.isEnd << "\n";
		for (int i = 0; i < 26; i++) {
			fout << snp.child[i] << (i == 25 ? "" : " ");
		}
		fout << "\n";
	}

	fout.close();
	std::cout << "Success: " << filepath << "\n";
}

void Trie::loadFromFile(const std::string& filepath) {
	TrieState state;
	if (filepath.empty()) return;

	std::ifstream fin(filepath);
	if (!fin.is_open()) {
		std::cerr << "Cannot open file: " << filepath << "\n";
		return;
	}

	int node_count = 0;
	if (fin >> state.next_ui_id >> node_count) {
		state.nodes.resize(node_count);

		for (int i = 0; i < node_count; ++i) {
			fin >> state.nodes[i].label;
			if (state.nodes[i].label == "-") state.nodes[i].label = "";

			fin >> state.nodes[i].ui_id
				>> state.nodes[i].cnt
				>> state.nodes[i].isEnd;

			for (int j = 0; j < 26; j++) {
				fin >> state.nodes[i].child[j];
			}
		}
	}

	fin.close();
	std::cout << "Success: " << filepath << "\n";
	loadState(state);
}

void Trie::clearWithoutRecorder(Node*& root) {
	if (root == nullptr) return;
	for (int i = 0; i < 26; i++) {
		clearWithoutRecorder(root->child[i]);
	}
	delete root;
	root = nullptr;
}

void Trie::applyOperation(const TrieOperation& operation, TrieRecorder& recorder) {
	using namespace std;
	using Target = TrieAnimationTarget;
	using Type = TrieAnimationType;
	using Command = TrieAnimationCommand;
	if (operation.type == TrieOperationType::Insert) {
		string x = operation.value;
		insert(x, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(4);
		return;
	}
	if (operation.type == TrieOperationType::Remove) {
		string x = operation.value;
		remove(x, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(9);
		return;
	}
	if (operation.type == TrieOperationType::Search) {
		string x = operation.value;
		search(x, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(6);
		return;
	}
	if (operation.type == TrieOperationType::Reset) {
		clear(root, nullptr, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(4);
		return;
	}
	if (operation.type == TrieOperationType::Save) {
		saveToFile(operation.file_path);
		return;
	}
	if (operation.type == TrieOperationType::Load) {
		loadFromFile(operation.file_path);
		return;
	}
}

void Trie::rawInit(const std::vector<std::string>& values) {
	for (int i = 0; i < 26; i++) clearWithoutRecorder(root->child[i]);
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
	using Command = TrieAnimationCommand;
	using Target = TrieAnimationTarget;
	using Type = TrieAnimationType;

	Node* tmp = root;
	int pre_id = -1;

	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	pre_id = root->ui_id;

	for (int i = 0; i < s.size(); i++) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(0);
		int nxt = s[i] - 'A';
		bool is_new = false;
		if (tmp->child[nxt] == nullptr) {
			std::string label = "";
			label += s[i];
			tmp->child[nxt] = new Node(label, next_ui_id++);
			is_new = true;
		}

		Node* next_node = tmp->child[nxt];
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(1);
		if (is_new) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(2);
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());

			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command::createSpawnNodeCommand(next_node->ui_id, tmp->ui_id, next_node->label, { 0, 200 }));
			recorder.addCommand(Command(Target::Node, Type::FadeIn, next_node->ui_id));

			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command::createSpawnEdgeCommand(tmp->ui_id, next_node->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, tmp->ui_id, next_node->ui_id));
		}

		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, next_node->ui_id));
		if (pre_id != -1) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));

		tmp = next_node;
		tmp->cnt++;
		pre_id = tmp->ui_id;
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(3);
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, tmp->ui_id));
	if (tmp->isEnd == 0){
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::SetEndMark, tmp->ui_id));
	}
	tmp->isEnd = 1;
}

void Trie::remove(const std::string& s, TrieRecorder& recorder) {
	using Command = TrieAnimationCommand;
	using Target = TrieAnimationTarget;
	using Type = TrieAnimationType;

	Node* tmp = root;
	int pre_id = -1;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	recorder.setHighlightedLine(0);
	pre_id = root->ui_id;
	for (int i = 0; i < s.size(); i++) {
		int nxt = s[i] - 'A';
		if (tmp->child[nxt] == nullptr) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, tmp->ui_id));
			recorder.setHighlightedLine(1);
			return;
		}
		pre_id = tmp->ui_id;
		tmp = tmp->child[nxt];
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, tmp->ui_id));
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
	}
	if (!tmp->isEnd) {
		//search(s, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, tmp->ui_id));
		recorder.setHighlightedLine(1);
		return;
	}
	std::vector<Node*> v;
	std::vector<bool> will_delete;
	tmp = root;
	v.push_back(tmp);
	will_delete.push_back(0);
	for (int i = 0; i < s.size(); i++) {
		int nxt = s[i] - 'A';
		if (tmp->child[nxt] == nullptr) return;
		tmp = tmp->child[nxt];
		tmp->cnt--;
		v.push_back(tmp);
		will_delete.push_back(tmp->cnt == 0);
	}
	std::cout << v.size() << " " << will_delete.size() << "\n";
	for (int i = 0; i < int(v.size()); i++) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(2);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, v[i]->ui_id));
	}
	bool set_not_end = tmp->isEnd == 1;
	tmp->isEnd = 0;
	for (int i = 0; i < v.size(); i++) {
		std::cout << i << " " << will_delete[i] << "\n";
	}
	for (int i = int(v.size()) - 1; i >= 0; i--) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(3);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(4);
		if (will_delete[i]) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(5);
			recorder.addNewPhase();
			recorder.setHighlightedLine(6);
			recorder.addCommand(Command(Target::Node, Type::FadeOut, v[i]->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, v[i - 1]->ui_id, v[i]->ui_id));
			v[i - 1]->child[s[i - 1] - 'A'] = nullptr;
			delete v[i];
			v[i] = nullptr;
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
		}
		else {
			recorder.addNewPhase();
			recorder.setHighlightedLine(7);
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, v[i]->ui_id));
			if (i == int(v.size()) - 1 && set_not_end) {
				recorder.addNewPhase();
				recorder.setHighlightedLine(8);
				recorder.addCommand(Command(Target::Node, Type::UnsetEndMark, v[i]->ui_id));
			}
		}
	}
}

void Trie::clear(Node*& root, Node* pa, TrieRecorder& recorder) {
	using Command = TrieAnimationCommand;
	using Target = TrieAnimationTarget;
	using Type = TrieAnimationType;

	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(0);
	if (root == nullptr) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(1);
		return;
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(2);
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));

	for (int i = 0; i < 26; i++) {
		if (root->child[i]) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(2);
			clear(root->child[i], root,recorder);
		}
	}

	if (pa) {
		recorder.addCommand(Command(Target::Edge, Type::FadeOut, pa->ui_id, root->ui_id));
	}
	if (root != this->root) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
		delete root;
		root = nullptr;
	}
	else {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
	}
}

void Trie::search(const std::string& s, TrieRecorder& recorder) {
	using Command = TrieAnimationCommand;
	using Target = TrieAnimationTarget;
	using Type = TrieAnimationType;

	Node* tmp = root;
	int pre_id = -1;

	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	pre_id = root->ui_id;

	for (int i = 0; i < s.size(); i++) {
		int nxt = s[i] - 'A';
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(0);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(1);
		if (tmp->child[nxt] == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(2);
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
			return;
		}

		Node* next_node = tmp->child[nxt];
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, next_node->ui_id));
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		tmp = next_node;
		pre_id = tmp->ui_id;
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(3);
	if (tmp->isEnd) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(4);
		recorder.addCommand(Command(Target::Node, Type::FoundedOn, tmp->ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::Wait, tmp->ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::FoundedOff, tmp->ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
		return;
	}

	recorder.addNewPhase();
	recorder.setHighlightedLine(5);
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre_id));
}