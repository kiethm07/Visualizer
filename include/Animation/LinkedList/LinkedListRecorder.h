#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationPhase.h>

class LinkedListRecorder {
public:
	LinkedListRecorder(){}
	void addCommand(const LinkedListAnimationCommand& command) {
		phases.back().commands.push_back(command);
	}
	void setHighlightedLine(int line) {
		highlighted_lines.back() = line;
	}
	void addNewPhase() {
		phases.push_back(LinkedListAnimationPhase());
		highlighted_lines.push_back(-1);
	}
	void clear() {
		phases.clear();
		highlighted_lines.clear();
	}
	//void setCurrentOperation(const ListOperation& operation) {
	//	current_operation = operation;
	//}
	void setHighlightedLineAsPrevious() {
		if (highlighted_lines.size() <= 1) return;
		int n = highlighted_lines.size();
		highlighted_lines[n - 1] = highlighted_lines[n - 2];
	}
	std::vector<LinkedListAnimationPhase> getPhases() const {
		return phases;
	}
	std::vector<int> getHighlightedLines() const {
		return highlighted_lines;
	}
private:
	std::vector<LinkedListAnimationPhase> phases;
	std::vector<int> highlighted_lines; //for pseudocode
	//ListOperation current_operation;
};