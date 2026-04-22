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
		highlighted_lines.push_back(-1);
	}
	void clear() {
		phases.clear();
		highlighted_lines.clear();
	}
	std::vector<AVLAnimationPhase> getPhases() const {
		return phases;
	}
	void setHighlightedLineAsPrevious() {
		if (highlighted_lines.size() <= 1) return;
		int n = highlighted_lines.size();
		highlighted_lines[n - 1] = highlighted_lines[n - 2];
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