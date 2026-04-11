#include <DataStructures/AVL.h>

AVL::AVL() : root(nullptr), next_ui_id(0) {}
AVL::~AVL() { clearWithoutRecorder(root); }

void AVL::applyOperation(const AVLOperation& operation, AVLRecorder& recorder) {
	using namespace std;
	if (operation.type == AVLOperationType::Insert) {
		int x = operation.value;
		insert(root, x, recorder);
		return;
	}
	if (operation.type == AVLOperationType::Remove) {
		int x = operation.value;
		remove(root, x, recorder);
		return;
	}
	if (operation.type == AVLOperationType::Search) {
		int x = operation.value;
		search(root, x, recorder);
		return;
	}
	if (operation.type == AVLOperationType::Reset) {
		clear(root, recorder);
		return;
	}
}

AVLState AVL::getState() const {
	AVLState state;
	if (!root) return state;
	std::vector<Node*> all_nodes;
	std::map<Node*, int> node_to_idx;
	auto collect = [&](auto self, Node* u) -> void {
		if (!u) return;
		node_to_idx[u] = (int)all_nodes.size();
		all_nodes.push_back(u);
		self(self, u->pLeft);
		self(self, u->pRight);
		};
	collect(collect, root);
	for (Node* u : all_nodes) {
		AVLNodeSnapshot snp;
		snp.label = std::to_string(u->val);
		snp.ui_id = u->ui_id;
		snp.size = u->size;
		snp.height = u->height;
		snp.leftChild = u->pLeft ? node_to_idx[u->pLeft] : -1;
		snp.rightChild = u->pRight ? node_to_idx[u->pRight] : -1;
		state.nodes.push_back(snp);
	}
	state.next_ui_id = this->next_ui_id;
	return state;
}

void AVL::loadState(const AVLState& state) {
	clearWithoutRecorder(root);
	if (state.nodes.empty()) {
		this->next_ui_id = state.next_ui_id;
		return;
	}
	int n = (int)state.nodes.size();
	std::vector<Node*> new_nodes(n);
	for (int i = 0; i < n; i++) {
		const auto& snp = state.nodes[i];
		new_nodes[i] = new Node(std::stoi(snp.label), snp.ui_id);
		new_nodes[i]->height = snp.height;
		new_nodes[i]->size = snp.size;
	}
	for (int i = 0; i < n; i++) {
		if (state.nodes[i].leftChild != -1) new_nodes[i]->pLeft = new_nodes[state.nodes[i].leftChild];
		if (state.nodes[i].rightChild != -1) new_nodes[i]->pRight = new_nodes[state.nodes[i].rightChild];
	}
	root = new_nodes[0];
	this->next_ui_id = state.next_ui_id;
}

void AVL::rawInit(const std::vector<int>& values) {
	AVLRecorder dummy;
	for (int x : values) insert(root, x, dummy);
}

int AVL::getHeight(Node* u) { 
	return u == nullptr ? 0 : u->height; 
}

int AVL::getSize(Node* u) { 
	return u == nullptr ? 0 : u->size; 
}

void AVL::updateState(Node*& u) {
	if (u == nullptr) return;
	u->height = std::max(getHeight(u->pLeft), getHeight(u->pRight)) + 1;
	u->size = getSize(u->pLeft) + getSize(u->pRight) + 1;
}

void AVL::rotateLeft(Node*& u) {
	if (u == nullptr) return;
	Node* tmp = u->pRight;
	if (tmp == nullptr) return;
	u->pRight = tmp->pLeft;
	tmp->pLeft = u;
	updateState(u);
	updateState(tmp);
	u = tmp;
}

void AVL::rotateRight(Node*& u) {
	if (u == nullptr) return;
	Node* tmp = u->pLeft;
	if (tmp == nullptr) return;
	u->pLeft = tmp->pRight;
	tmp->pRight = u;
	updateState(u);
	updateState(tmp);
	u = tmp;
}

void AVL::balance(Node*& u) {
	if (u == nullptr) return;
	int difference = getHeight(u->pLeft) - getHeight(u->pRight);
	if (difference >= -1 && difference <= 1) return;
	if (difference > 1) {
		Node*& t = u->pLeft;
		if (t && getHeight(t->pLeft) < getHeight(t->pRight)) { rotateLeft(t); }
		rotateRight(u);
		return;
	}
	Node*& t = u->pRight;
	if (t && getHeight(t->pRight) < getHeight(t->pLeft)) { rotateRight(t); }
	rotateLeft(u);
}

void AVL::insert(Node*& root, int x, AVLRecorder& recorder) {
	if (root == nullptr) {
		root = new Node(x, next_ui_id++);
		return;
	}
	if (root->val == x) return;
	if (root->val < x) insert(root->pRight, x, recorder);
	else insert(root->pLeft, x, recorder);
	updateState(root);
	balance(root);
}

void AVL::remove(Node*& root, int x, AVLRecorder& recorder) {
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
		remove(root->pRight, tmp->val, recorder);
		updateState(root);
		balance(root);
		return;
	}
	if (root->val < x) remove(root->pRight, x, recorder);
	else remove(root->pLeft, x, recorder);
	updateState(root);
	balance(root);
}

bool AVL::search(Node* root, int x, AVLRecorder& recorder) {
	if (root == nullptr) return false;
	if (root->val == x) return true;
	if (root->val < x) return search(root->pRight, x, recorder);
	return search(root->pLeft, x, recorder);
}

void AVL::clear(Node*& root, AVLRecorder& recorder) {
	if (root == nullptr) return;
	clear(root->pLeft, recorder);
	clear(root->pRight, recorder);
	delete root;
	root = nullptr;
}

void AVL::clearWithoutRecorder(Node*& root) {
	if (root == nullptr) return;
	clearWithoutRecorder(root->pLeft);
	clearWithoutRecorder(root->pRight);
	delete root;
	root = nullptr;
}