#include <DataStructures/AVL.h>
#include <map>
#include <algorithm>

AVL::AVL() : root(nullptr), next_ui_id(0) {}
AVL::~AVL() { clearWithoutRecorder(root); }

void AVL::applyOperation(const AVLOperation& operation, AVLRecorder& recorder) {
	using namespace std;
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
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
		snp.value = u->val;
		snp.ui_id = u->ui_id;
		snp.size = u->size;
		snp.height = u->height;
		snp.leftChild = u->pLeft ? node_to_idx[u->pLeft] : -1;
		snp.rightChild = u->pRight ? node_to_idx[u->pRight] : -1;
		snp.parent = u->pParent ? node_to_idx[u->pParent] : -1;
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
		new_nodes[i] = new Node(snp.value, snp.ui_id);
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
	clearWithoutRecorder(root);
	for (int x : values) insert(root, x, dummy);
}

int AVL::getHeight(Node* u) { return u == nullptr ? 0 : u->height; }
int AVL::getSize(Node* u) { return u == nullptr ? 0 : u->size; }

void AVL::updateState(Node*& u) {
	if (u == nullptr) return;
	u->height = std::max(getHeight(u->pLeft), getHeight(u->pRight)) + 1;
	u->size = getSize(u->pLeft) + getSize(u->pRight) + 1;
}

void AVL::rotateLeft(Node*& u, AVLRecorder& recorder) {
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	using Command = AVLAnimationCommand;
	if (u == nullptr) return;
	Node* tmp = u->pRight;
	if (tmp == nullptr) return;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Edge, Type::FadeOut, u->ui_id, tmp->ui_id));
	if (tmp->pLeft) recorder.addCommand(Command(Target::Edge, Type::FadeOut, tmp->ui_id, tmp->pLeft->ui_id));
	if (u->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, u->pParent->ui_id, u->ui_id));
	u->pRight = tmp->pLeft;
	if (u->pRight != nullptr) u->pRight->pParent = u;
	tmp->pLeft = u;
	tmp->pParent = u->pParent;
	u->pParent = tmp;
	updateState(u);
	updateState(tmp);
	u = tmp;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
	recorder.addState(getState());

	recorder.addNewPhase();
	recorder.addCommand(Command::createSpawnEdgeCommand(u->ui_id, u->pLeft->ui_id));
	recorder.addCommand(Command(Target::Edge, Type::FadeIn, u->ui_id, u->pLeft->ui_id));
	if (u->pParent) {
		recorder.addCommand(Command::createSpawnEdgeCommand(u->pParent->ui_id, u->ui_id));
		recorder.addCommand(Command(Target::Edge, Type::FadeIn, u->pParent->ui_id, u->ui_id));
	}
	if (Node* tmp = u->pLeft) {
		if (tmp->pRight) {
			recorder.addCommand(Command::createSpawnEdgeCommand(tmp->ui_id, tmp->pRight->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, tmp->ui_id, tmp->pRight->ui_id));
		}
	}
}

void AVL::rotateRight(Node*& u, AVLRecorder& recorder) {
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	using Command = AVLAnimationCommand;
	if (u == nullptr) return;
	Node* tmp = u->pLeft;
	if (tmp == nullptr) return;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Edge, Type::FadeOut, u->ui_id, tmp->ui_id));
	if (tmp->pRight) recorder.addCommand(Command(Target::Edge, Type::FadeOut, tmp->ui_id, tmp->pRight->ui_id));
	if (u->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, u->pParent->ui_id, u->ui_id));
	u->pLeft = tmp->pRight;
	if (u->pLeft != nullptr) u->pLeft->pParent = u;
	tmp->pRight = u;
	tmp->pParent = u->pParent;
	u->pParent = tmp;
	updateState(u);
	updateState(tmp);
	u = tmp;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
	recorder.addState(getState());

	recorder.addNewPhase();
	recorder.addCommand(Command::createSpawnEdgeCommand(u->ui_id, u->pRight->ui_id));
	recorder.addCommand(Command(Target::Edge, Type::FadeIn, u->ui_id, u->pRight->ui_id));
	if (u->pParent) {
		//recorder.addNewPhase();
		recorder.addCommand(Command::createSpawnEdgeCommand(u->pParent->ui_id, u->ui_id));
		recorder.addCommand(Command(Target::Edge, Type::FadeIn, u->pParent->ui_id, u->ui_id));
	}
	if (Node* tmp = u->pRight) {
		//recorder.addNewPhase();
		if (tmp->pLeft) {
			recorder.addCommand(Command::createSpawnEdgeCommand(tmp->ui_id, tmp->pLeft->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, tmp->ui_id, tmp->pLeft->ui_id));
		}
	}
}

//void AVL::rotateRight(Node*& u, AVLRecorder& recorder) {
//	using Target = AVLAnimationTarget;
//	using Type = AVLAnimationType;
//	using Command = AVLAnimationCommand;
//	if (u == nullptr) return;
//	Node* tmp = u->pLeft;
//	if (tmp == nullptr) return;
//	u->pLeft = tmp->pRight;
//	tmp->pRight = u;
//	updateState(u);
//	updateState(tmp);
//	u = tmp;
//	recorder.addNewPhase();
//	recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
//	recorder.addState(getState());
//}

void AVL::balance(Node*& u, AVLRecorder& recorder) {
	if (u == nullptr) return;
	int difference = getHeight(u->pLeft) - getHeight(u->pRight);
	if (difference >= -1 && difference <= 1) return;
	if (difference > 1) {
		Node*& t = u->pLeft;
		if (t && getHeight(t->pLeft) < getHeight(t->pRight)) { rotateLeft(t, recorder); }
		rotateRight(u, recorder);
		return;
	}
	Node*& t = u->pRight;
	if (t && getHeight(t->pRight) < getHeight(t->pLeft)) { rotateRight(t, recorder); }
	rotateLeft(u, recorder);
	//using Target = AVLAnimationTarget;
	//using Type = AVLAnimationType;
	//using Command = AVLAnimationCommand;
	//recorder.addNewPhase();
	//recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
	//recorder.addState(getState());
}

void AVL::insert(Node*& root, int x, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	if (root == nullptr) {
		//Case init
		root = new Node(x, next_ui_id++);
		root->pParent = nullptr;
		recorder.addNewPhase();
		recorder.addCommand(Command::createSpawnNodeCommand(root->ui_id, -1, x, {}));
		recorder.addCommand(Command(Target::Node, Type::FadeIn, root->ui_id));
		return;
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	//if (root->pParent) recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->pParent->ui_id));
	if (root->val == x) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
		return;
	}
	int pre_id = root->ui_id;
	if (root->val < x) {
		if (root->pRight == nullptr) {
			root->pRight = new Node(x, next_ui_id++);
			root->pRight->pParent = root;
			recorder.addNewPhase();
			recorder.addCommand(Command::createSpawnNodeCommand(root->pRight->ui_id, pre_id, x, { 100, 150 }));
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->pRight->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command::createSpawnEdgeCommand(pre_id, root->pRight->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->ui_id, root->pRight->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
		}
		else insert(root->pRight, x, recorder);
	}
	else {
		if (root->pLeft == nullptr) {
			root->pLeft = new Node(x, next_ui_id++);
			root->pLeft->pParent = root;
			recorder.addNewPhase();
			recorder.addCommand(Command::createSpawnNodeCommand(root->pLeft->ui_id, pre_id, x, { -100, 150 }));
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->pLeft->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command::createSpawnEdgeCommand(pre_id, root->pLeft->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->ui_id, root->pLeft->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
		}
		else insert(root->pLeft, x, recorder);
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
	updateState(root);
	balance(root, recorder);
}

void AVL::remove(Node*& root, int x, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	if (root == nullptr) return;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	if (root->val == x) {
		std::cout << root->val << " removed !\n";
		std::cout << (root->pParent ? (root->pParent->val) : -1) << " parent! \n";
		std::cout << getHeight(root) << " height!\n";
		if (root->pRight == nullptr && root->pLeft == nullptr) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			delete root;
			root = nullptr;
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			return;
		}
		else if (root->pRight == nullptr) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->ui_id, root->pLeft->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			Node* tmp = root->pLeft;
			tmp->pParent = root->pParent;
			delete root;
			root = tmp;
			if (root->pParent) {
				recorder.addNewPhase();
				recorder.addCommand(Command::createSpawnEdgeCommand(root->pParent->ui_id, root->ui_id));
				recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->pParent->ui_id, root->ui_id));
			}
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			return;
		}
		else if (root->pLeft == nullptr) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->ui_id, root->pRight->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			Node* tmp = root->pRight;
			tmp->pParent = root->pParent;
			delete root;
			root = tmp;
			if (root->pParent) {
				recorder.addNewPhase();
				recorder.addCommand(Command::createSpawnEdgeCommand(root->pParent->ui_id, root->ui_id));
				recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->pParent->ui_id, root->ui_id));
			}
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			return;
		}
		else {
			Node* tmp = root->pRight;
			while (tmp->pLeft != nullptr) {
				tmp = tmp->pLeft;
			}
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
			recorder.addCommand(Command(Target::Node, Type::HighlightOn, tmp->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command::createChangeValueCommand(root->ui_id, tmp->val));
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->ui_id));
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, tmp->ui_id));
			root->val = tmp->val;
			remove(root->pRight, tmp->val, recorder);
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			updateState(root);
			balance(root, recorder);
			return;
		}
	}
	else if (root->val < x) remove(root->pRight, x, recorder);
	else remove(root->pLeft, x, recorder);
	if (root) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
		updateState(root);
		balance(root, recorder);
	}
}

bool AVL::search(Node* root, int x, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	if (root == nullptr) return false;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	bool found = false;
	if (root->val == x) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::FoundedOn, root->ui_id));
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, root->ui_id));
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::FoundedOff, root->ui_id));
		found = true;
	}
	else if (root->val < x) found = search(root->pRight, x, recorder);
	else found = search(root->pLeft, x, recorder);
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
	return found;
}

void AVL::clear(Node*& root, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	if (root == nullptr) return;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	clear(root->pLeft, recorder);
	clear(root->pRight, recorder);
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
	if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
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