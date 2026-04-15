#pragma once
#include <vector>
#include <Animation/AVL/AVLAnimationPhase.h>

class AVLRecorder {
public:
	AVLRecorder() {}
	void addCommand(const AVLAnimationCommand& command) {
		phases.back().commands.push_back(command);
	}
	void addState(const AVLState& snapshot) {
		phases.back().snapshot = snapshot;
	}
	void addNewPhase() {
		phases.push_back(AVLAnimationPhase());
	}
	void clear() {
		phases.clear();
	}
	std::vector<AVLAnimationPhase> getPhases() const {
		return phases;
	}
private:
	std::vector<AVLAnimationPhase> phases;
};