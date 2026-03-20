#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationPhase.h>

class LinkedListRecorder {
public:
	LinkedListRecorder(){}
	void addCommand(const LinkedListAnimationCommand& command) {
		if (phases.empty()) {
			phases.push_back(LinkedListAnimationPhase());
		}
		phases.back().commands.push_back(command);
	}
	void addNewPhase() {
		phases.push_back(LinkedListAnimationPhase());
	}
	void clear() {
		phases.clear();
	}
private:
	std::vector<LinkedListAnimationPhase> phases;
};