#pragma once
#include <vector>
#include <Animation/Hashmap/HashmapAnimationPhase.h>

class HashmapRecorder {
public:
	HashmapRecorder() {}
	void addCommand(const HashmapAnimationCommand& command) {
		phases.back().commands.push_back(command);
	}
	void addNewPhase() {
		phases.push_back(HashmapAnimationPhase());
		highlighted_lines.push_back(-1);
	}
	void clear() {
		phases.clear();
		highlighted_lines.clear();
	}
	std::vector<HashmapAnimationPhase> getPhases() const {
		return phases;
	}
	void setHighlightedLine(int line) {
		highlighted_lines.back() = line;
	}
	std::vector<int> getHighlightedLines() const {
		return highlighted_lines;
	}
private:
	std::vector<HashmapAnimationPhase> phases;
	std::vector<int> highlighted_lines;
};