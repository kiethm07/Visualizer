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
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(8);
		return;
	}
	if (operation.type == AVLOperationType::Remove) {
		int x = operation.value;
		remove(root, x, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(10);
		return;
	}
	if (operation.type == AVLOperationType::Search) {
		int x = operation.value;
		search(root, x, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(7);
		return;
	}
	if (operation.type == AVLOperationType::Reset) {
		clear(root, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(4);
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
		new_nodes[i]->pLeft = new_nodes[i]->pRight = new_nodes[i]->pParent = nullptr;
	}
	for (int i = 0; i < n; i++) {
		if (state.nodes[i].leftChild != -1) new_nodes[i]->pLeft = new_nodes[state.nodes[i].leftChild];
		if (state.nodes[i].rightChild != -1) new_nodes[i]->pRight = new_nodes[state.nodes[i].rightChild];
		if (new_nodes[i]->pLeft) new_nodes[i]->pLeft->pParent = new_nodes[i];
		if (new_nodes[i]->pRight) new_nodes[i]->pRight->pParent = new_nodes[i];
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
	recorder.setHighlightedLineAsPrevious();
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
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
	recorder.addState(getState());

	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
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
	recorder.setHighlightedLineAsPrevious();
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
	recorder.setHighlightedLineAsPrevious();
	recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
	recorder.addState(getState());

	recorder.addNewPhase();
	recorder.setHighlightedLineAsPrevious();
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
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	if (root == nullptr) {
		//Case init
		root = new Node(x, next_ui_id++);
		root->pParent = nullptr;
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command::createSpawnNodeCommand(root->ui_id, -1, x, {}));
		recorder.addCommand(Command(Target::Node, Type::FadeIn, root->ui_id));
		return;
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	recorder.setHighlightedLine(2);
	//if (root->pParent) recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->pParent->ui_id));
	if (root->val == x) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
		return;
	}
	int pre_id = root->ui_id;
	recorder.addNewPhase();
	recorder.setHighlightedLine(4);
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	if (root->val < x) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(5);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		if (root->pRight == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(0);
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			root->pRight = new Node(x, next_ui_id++);
			root->pRight->pParent = root;
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command::createSpawnNodeCommand(root->pRight->ui_id, pre_id, x, { 100, 150 }));
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->pRight->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command::createSpawnEdgeCommand(pre_id, root->pRight->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->ui_id, root->pRight->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
		}
		else insert(root->pRight, x, recorder);
	}
	else {
		recorder.addNewPhase();
		recorder.setHighlightedLine(6);
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		if (root->pLeft == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(0);
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			root->pLeft = new Node(x, next_ui_id++);
			root->pLeft->pParent = root;
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command::createSpawnNodeCommand(root->pLeft->ui_id, pre_id, x, { -100, 150 }));
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->pLeft->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command::createSpawnEdgeCommand(pre_id, root->pLeft->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->ui_id, root->pLeft->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLine(1);
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
		}
		else insert(root->pLeft, x, recorder);
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
	updateState(root);
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(7);
	balance(root, recorder);
}

void AVL::remove(Node*& root, int x, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(0);
	if (root == nullptr) return;
	recorder.addNewPhase();
	recorder.setHighlightedLine(1);
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	if (root->val == x) {
		//std::cout << root->val << " removed !\n";
		//std::cout << (root->pParent ? (root->pParent->val) : -1) << " parent! \n";
		//std::cout << getHeight(root) << " height!\n";
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(2);
		if (root->pRight == nullptr && root->pLeft == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(3);
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			delete root;
			root = nullptr;
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			return;
		}
		else if (root->pRight == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(3);
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->ui_id, root->pLeft->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			Node* tmp = root->pLeft;
			tmp->pParent = root->pParent;
			delete root;
			root = tmp;
			if (root->pParent) {
				recorder.addNewPhase();
				recorder.setHighlightedLineAsPrevious();
				recorder.addCommand(Command::createSpawnEdgeCommand(root->pParent->ui_id, root->ui_id));
				recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->pParent->ui_id, root->ui_id));
			}
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			recorder.addState(getState());
			return;
		}
		else if (root->pLeft == nullptr) {
			recorder.addNewPhase();
			recorder.setHighlightedLine(3);
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->ui_id, root->pRight->ui_id));
			if (root->pParent) recorder.addCommand(Command(Target::Edge, Type::FadeOut, root->pParent->ui_id, root->ui_id));
			Node* tmp = root->pRight;
			tmp->pParent = root->pParent;
			delete root;
			root = tmp;
			if (root->pParent) {
				recorder.addNewPhase();
				recorder.setHighlightedLineAsPrevious();
				recorder.addCommand(Command::createSpawnEdgeCommand(root->pParent->ui_id, root->ui_id));
				recorder.addCommand(Command(Target::Edge, Type::FadeIn, root->pParent->ui_id, root->ui_id));
			}
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
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
			recorder.setHighlightedLine(4);
			//recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
			recorder.addCommand(Command(Target::Node, Type::HighlightOn, tmp->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::FadeOut, root->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command::createChangeValueCommand(root->ui_id, tmp->val));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			recorder.addCommand(Command(Target::Node, Type::FadeIn, root->ui_id));
			recorder.addNewPhase();
			recorder.setHighlightedLineAsPrevious();
			//recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, tmp->ui_id));
			root->val = tmp->val;
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(5);
			remove(root->pRight, tmp->val, recorder);
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
			//recorder.addCommand(Command(Target::All, Type::Reconstruct, -1));
			//recorder.addState(getState());
			updateState(root);
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(9);
			balance(root, recorder);
			return;
		}
	}
	else {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(6);
		if (root->val < x) {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(7);
			remove(root->pRight, x, recorder);
		}
		else {
			recorder.addNewPhase();
			recorder.addCommand(Command(Target::Node, Type::Wait, -1));
			recorder.setHighlightedLine(8);
			remove(root->pLeft, x, recorder);
		}
	}
	if (root) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
		updateState(root);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(9);
		balance(root, recorder);
	}
}

bool AVL::search(Node* root, int x, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(0);
	if (root == nullptr) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(1);
		return false;
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(2);
	bool found = false;
	if (root->val == x) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::FoundedOn, root->ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::Wait, root->ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLineAsPrevious();
		recorder.addCommand(Command(Target::Node, Type::FoundedOff, root->ui_id));
		return true;
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(4);
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	if (root->val < x) {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(5);
		found = search(root->pRight, x, recorder);
	}
	else {
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::Wait, -1));
		recorder.setHighlightedLine(6);
		found = search(root->pLeft, x, recorder);
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOff, root->ui_id));
	return found;
}

void AVL::clear(Node*& root, AVLRecorder& recorder) {
	using Command = AVLAnimationCommand;
	using Target = AVLAnimationTarget;
	using Type = AVLAnimationType;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(0);
	if (root == nullptr) return;
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, root->ui_id));
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(1);
	clear(root->pLeft, recorder);
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::Node, Type::Wait, -1));
	recorder.setHighlightedLine(2);
	clear(root->pRight, recorder);
	recorder.addNewPhase();
	recorder.setHighlightedLine(3);
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