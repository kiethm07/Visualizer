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

void LinkedList::rawInit(const std::vector<int>& values) {
    clearWithoutRecorder(pHead);
    next_ui_id = 0;
    Node* cur = pHead;
    for (int i : values) {
        if (pHead == nullptr) {
            pHead = new Node(i, next_ui_id++);
            cur = pHead;
        }
        else {
            cur->pNext = new Node(i, next_ui_id++);
            cur = cur->pNext;
        }
    }
}

void LinkedList::clearWithoutRecorder(LinkedList::Node*& pHead) {
    if (pHead == nullptr) return;
    for (Node* cur = pHead; cur != nullptr;) {
        Node* tmp = cur;
        cur = cur->pNext;
        delete tmp;
        tmp = nullptr;
    }
    pHead = nullptr;
    next_ui_id = 0;
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
    if (operation.type == ListOperationType::Remove) {
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
    if (operation.type == ListOperationType::Search) {
        int x = operation.value;
        search(pHead, x, recorder);
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
        //recorder.addNewPhase();
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::FadeOut, pHead->ui_id, pNext_ui_id));
	}
    delete pHead;
    pHead = nullptr;
}

LinkedList::~LinkedList() {
    clearWithoutRecorder(pHead);
}

void LinkedList::insert(LinkedList::Node*& pHead, const int& x, const int& k, LinkedListRecorder& recorder) {
    using Command = LinkedListAnimationCommand;
    using Target = LinkedListAnimationTarget;
    using Type = LinkedListAnimationType;
    if (k < 0) return;
    if (k == 0) {
        insertFirst(pHead, x, recorder);
        return;
    }
    Node* cur = pHead;
    Node* pre = nullptr;
    for (int i = 0; i < k && cur != nullptr; i++) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
        if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        if (i == k - 1) break;
        pre = cur;
        cur = cur->pNext;
    }
    if (cur == nullptr && pre != nullptr) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        return;
    }
    if (cur == nullptr) return;
    Node* tmp = new Node(x, next_ui_id++);
    recorder.addNewPhase();
    recorder.addCommand(Command::createSpawnNodeCommand(tmp->ui_id, cur->ui_id, x, { 200, 200 }));
    recorder.addCommand(Command(Target::Node, Type::FadeIn, tmp->ui_id));
    recorder.addNewPhase();
    for (Node* n = cur->pNext; n != nullptr; n = n->pNext) {
        recorder.addCommand(Command(Target::Node, Type::Move, n->ui_id));
    }
    tmp->pNext = cur->pNext;
    if (tmp->pNext) {
        recorder.addNewPhase();
        recorder.addCommand(Command::createSpawnEdgeCommand(tmp->ui_id, tmp->pNext->ui_id));
        recorder.addCommand(Command(Target::Edge, Type::FadeIn, tmp->ui_id, tmp->pNext->ui_id));
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Edge, Type::FadeOut, cur->ui_id, tmp->pNext->ui_id));
    }
    cur->pNext = tmp;
    recorder.addNewPhase();
    recorder.addCommand(Command::createSpawnEdgeCommand(cur->ui_id, tmp->ui_id));
    recorder.addCommand(Command(Target::Edge, Type::FadeIn, cur->ui_id, tmp->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::Move, LinkedListMoveDirection::Up, tmp->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::HighlightOff, cur->ui_id));
}
    
void LinkedList::insertFirst(LinkedList::Node*& pHead, int x, LinkedListRecorder& recorder) {
    recorder.addNewPhase();
    for (Node* cur = pHead; cur != nullptr; cur = cur->pNext) {
		//std::cout << "Move node " << cur->val << " with ui_id " << cur->ui_id << "\n";
        recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Node, LinkedListAnimationType::Move, cur->ui_id));
        //if (cur->pNext) {
        //    recorder.addCommand(LinkedListAnimationCommand(LinkedListAnimationTarget::Edge, LinkedListAnimationType::Move, cur->ui_id, cur->pNext->ui_id));
        //}
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
    using Command = LinkedListAnimationCommand;
    using Target = LinkedListAnimationTarget;
    using Type = LinkedListAnimationType;
    if (k < 0) return;
    //if (k == 0) {
    //    removeFirst(pHead, recorder);
    //    return;
    //}
    Node* cur = pHead;
    Node* pre = nullptr;
    for (int i = 0; i < k && cur != nullptr; i++) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
        if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        pre = cur;
        cur = cur->pNext;
    }
    //if (cur == nullptr && pre != nullptr) {
    //    recorder.addNewPhase();
    //    recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
    //}
    if (cur == nullptr && pre == nullptr) return;
    if (cur == nullptr && pre != nullptr) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        return;
    }
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
    if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::FadeOut, cur->ui_id));
    if (cur->pNext) {
        recorder.addCommand(Command(Target::Edge, Type::FadeOut, cur->ui_id, cur->pNext->ui_id));
    }
    else {
        if (pre) recorder.addCommand(Command(Target::Edge, Type::FadeOut, pre->ui_id, cur->ui_id));
    }
    recorder.addNewPhase();
    for (Node* n = cur->pNext; n != nullptr; n = n->pNext) {
        recorder.addCommand(Command(Target::Node, Type::Move, LinkedListMoveDirection::Left, n->ui_id));
    }
    if (pre != nullptr) {
        if (cur->pNext != nullptr) {
            recorder.addNewPhase();
            recorder.addCommand(Command::createSpawnEdgeCommand(pre->ui_id, cur->pNext->ui_id));
            recorder.addCommand(Command(Target::Edge, Type::FadeIn, pre->ui_id, cur->pNext->ui_id));
        }
        pre->pNext = cur->pNext;
    }
    else {
        pHead = cur->pNext;
    }
    delete cur;
    cur = nullptr;
}

void LinkedList::update(Node*& pHead, const int& x, const int& k, LinkedListRecorder& recorder) {
    using Command = LinkedListAnimationCommand;
    using Target = LinkedListAnimationTarget;
    using Type = LinkedListAnimationType;
    if (k < 0) return;
    Node* cur = pHead;
    Node* pre = nullptr;
    for (int i = 0; i < k && cur != nullptr; i++) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
        if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        pre = cur;
        cur = cur->pNext;
    }
    if (cur == nullptr) {
        if (pre != nullptr) {
            recorder.addNewPhase();
            recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        }
        return;
    }
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
    if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::FadeOut, cur->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command::createChangeValueCommand(cur->ui_id, x));
    //std::cout << " " << x << "\n";
    cur->val = x;
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::FadeIn, cur->ui_id));
    recorder.addNewPhase();
    recorder.addCommand(Command(Target::Node, Type::HighlightOff, cur->ui_id));
}

void LinkedList::search(Node*& pHead, int x, LinkedListRecorder& recorder) {
    using Command = LinkedListAnimationCommand;
    using Target = LinkedListAnimationTarget;
    using Type = LinkedListAnimationType;
    Node* cur = pHead;
    Node* pre = nullptr;
    while (cur != nullptr) {
        recorder.addNewPhase();
        recorder.addCommand(Command(Target::Node, Type::HighlightOn, cur->ui_id));
        if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
        if (cur->val == x) {
            std::cout << "Found " << x << "\n";
            recorder.addNewPhase();
            recorder.addCommand(Command(Target::Node, Type::FoundedOn, cur->ui_id));
            recorder.addNewPhase();
            recorder.addCommand(Command(Target::Node, Type::Wait, cur->ui_id));
            recorder.addNewPhase();
            recorder.addCommand(Command(Target::Node, Type::FoundedOff, cur->ui_id));
            return;
        }
        pre = cur;
        cur = cur->pNext;
    }
    std::cout << "No " << x << "!\n";
    if (pre) recorder.addCommand(Command(Target::Node, Type::HighlightOff, pre->ui_id));
}