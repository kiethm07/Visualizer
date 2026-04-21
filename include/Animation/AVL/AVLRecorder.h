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
		highlighted_lines.clear();
	}
	std::vector<AVLAnimationPhase> getPhases() const {
		return phases;
	}
	void setHighlightedLine(int index) {
		highlighted_lines.back() = index;
	}
	std::vector<int> getHighlightedLines() const {
		return highlighted_lines;
	}
private:
	std::vector<AVLAnimationPhase> phases;
	std::vector<int> highlighted_lines;
};