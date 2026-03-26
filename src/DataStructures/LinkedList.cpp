#include <DataStructures/LinkedList.h>
#include <iostream>

LinkedListState LinkedList::getState() {
    LinkedListState state;
    state.next_ui_id = this->next_ui_id;
    for (LinkedList::Node* cur = pHead; cur != nullptr; cur = cur->pNext) {
        state.value.push_back(cur->val);
        state.ui_id.push_back(cur->ui_id);
    }
    return state;
}

void LinkedList::clearWithoutRecorder(LinkedList::Node*& pHead) {
    if (pHead == nullptr) return;
    clearWithoutRecorder(pHead->pNext);
    delete pHead;
    pHead = nullptr;
}

void LinkedList::loadState(const LinkedListState& state) {
    this->next_ui_id = state.next_ui_id;
    clearWithoutRecorder(pHead);
    Node* cur = nullptr;
    for (int i = 0; i < state.value.size(); i++) {
        int val = state.value[i];
        int ui_id = state.ui_id[i];
        if (i == 0) {
            pHead = new Node(val, ui_id);
            cur = pHead;
        }
        else {
            cur->pNext = new Node(val, ui_id);
            cur = cur->pNext;
        }
    }
}

void LinkedList::applyOperation(const ListOperation& operation, LinkedListRecorder& recorder) {
    if (operation.type == ListOperationType::InsertSingle) {
        int x = operation.value;
        int k = operation.position;
        insert(pHead, x, k, recorder);
        return;
    }
    if (operation.type == ListOperationType::InsertMultiple) {

        return;
    }
    if (operation.type == ListOperationType::Delete) {
        int k = operation.position;
        remove(pHead, k, recorder);
        return;
    }
    if (operation.type == ListOperationType::Update) {
        int x = operation.value;
        int k = operation.position;
        update(pHead, x, k, recorder);
        return;
    }
    if (operation.type == ListOperationType::Reset) {
        clear(pHead, recorder);
        return;
    }
}

LinkedList::LinkedList() :
    pHead(nullptr), next_ui_id(0) {
}

void LinkedList::clear(LinkedList::Node*& pHead, LinkedListRecorder& recorder) {
    if (pHead == nullptr) return;
	int pNext_ui_id = pHead->pNext ? pHead->pNext->ui_id : -1;
    clear(pHead->pNext, recorder);
    recorder.addNewPhase();
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::HighlightOn, pHead->ui_id));
    recorder.addNewPhase();
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::FadeOut, pHead->ui_id));
    if (pNext_ui_id != -1) {
        recorder.addNewPhase();
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeOut, pHead->ui_id, pNext_ui_id));
	}
    delete pHead;
    pHead = nullptr;
}

LinkedList::~LinkedList() {
    clearWithoutRecorder(pHead);
}

void LinkedList::insert(LinkedList::Node*& pHead, const int& x, const int& k, LinkedListRecorder& recorder) {
    if (k < 0) return;
    if (k == 0) {
        insertFirst(pHead, x, recorder);
        return;
    }
    Node* cur = pHead;
    int preId = -1;
    for (int i = 0; i < k - 1 && cur != nullptr; i++) {
        //unhightlight node i - 1
        //highlight node i
        //wait
        if (preId != -1) {
            recorder.addNewPhase();
            recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::HighlightOff, preId));
		}
        recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::HighlightOn, cur->ui_id));
        preId = cur->ui_id;
        cur = cur->pNext;
        recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::Wait, cur ? cur->ui_id : -1));
    }
    if (cur == nullptr) {
        recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::HighlightOff, preId));
		return;
    }
	///Move ALL nodes and edges from cur to the right
    recorder.addNewPhase();
    for (Node* cur_ui = cur ->pNext; cur_ui != nullptr; cur_ui = cur_ui->pNext) {
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::Move, cur_ui->ui_id));
        if (cur_ui->pNext) {
            recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::Move, cur_ui->ui_id, cur_ui->pNext->ui_id));
		}
	}
    Node* tmp = new Node(x, next_ui_id++);
    recorder.addNewPhase();
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::FadeIn, tmp->ui_id));
    tmp->pNext = cur->pNext;
    if (cur->pNext) {
        recorder.addNewPhase();
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeIn, tmp->ui_id, cur->pNext->ui_id));
    }
    cur->pNext = tmp;
    if (tmp->pNext) {
		recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeOut, cur->ui_id, tmp->pNext->ui_id));
    }
    recorder.addNewPhase();
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeIn, cur->ui_id, tmp->ui_id));
	recorder.addNewPhase();
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::HighlightOff, cur->ui_id));
}
    
void LinkedList::insertFirst(LinkedList::Node*& pHead, int x, LinkedListRecorder& recorder) {
    recorder.addNewPhase();
    for (Node* cur = pHead; cur != nullptr; cur = cur->pNext) {
		//std::cout << "Move node " << cur->val << " with ui_id " << cur->ui_id << "\n";
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::Move, cur->ui_id));
        if (cur->pNext) {
            recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::Move, cur->ui_id, cur->pNext->ui_id));
        }
    }
    Node* tmp = new Node(x, next_ui_id++);
	recorder.addNewPhase();
    recorder.addCommand(LinkedListAnimationCommand::createSpawnNodeCommand(tmp->ui_id, pHead ? pHead->ui_id : -1, x, { 0, 200 }));
	recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::FadeIn, tmp->ui_id));
    tmp->pNext = pHead;
    if (pHead) {
		recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand::createSpawnEdgeCommand(tmp->ui_id, pHead->ui_id));
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeIn, tmp->ui_id, pHead->ui_id));
        recorder.addNewPhase();
		recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::Move, LinkedListMoveDirection::Up, tmp->ui_id));
    }
    pHead = tmp;
}

void LinkedList::print() {
    Node* cur = pHead;
    while (cur != nullptr) {
        std::cout << cur->val << " ";
        cur = cur->pNext;
    }
    return;
}

void LinkedList::remove(LinkedList::Node*& pHead, int k, LinkedListRecorder& recorder) {
    if (k == 0) {
        removeFirst(pHead, recorder);
        return;
    }
    Node* cur = pHead;
    for (int i = 0; i < k && cur; i++) {
        cur = cur->pNext;
    }
    if (cur == nullptr) return;
    Node* tmp = cur->pNext;
    cur->pNext = tmp->pNext;
    delete tmp;
    tmp = nullptr;
}

void LinkedList::removeFirst(LinkedList::Node*& pHead, LinkedListRecorder& recorder) {
    if (pHead == nullptr) return;
    Node* tmp = pHead;
    pHead = pHead->pNext;
    delete tmp;
    tmp = nullptr;
}

void LinkedList::update(LinkedList::Node*& pHead, int x, int k, LinkedListRecorder& recorder) {
    Node* cur = pHead;
    for (int i = 0; i < k && cur; i++) {
        cur = cur->pNext;
    }
    if (cur == nullptr) return;
    cur->val = x;
}