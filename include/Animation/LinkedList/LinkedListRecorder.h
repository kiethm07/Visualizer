#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationPhase.h>

class LinkedListRecorder {
public:
	LinkedListRecorder(){}
	void addCommand(const LinkedListAnimationCommand& command) {
		phases.back().commands.push_back(command);
	}
	void addNewPhase() {
		phases.push_back(LinkedListAnimationPhase());
	}
	void clear() {
		phases.clear();
	}
	std::vector<LinkedListAnimationPhase> getPhases() const {
		return phases;
	}
private:
	std::vector<LinkedListAnimationPhase> phases;
};