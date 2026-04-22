#pragma once
#include <vector>
#include <Animation/Trie/TrieAnimationPhase.h>

class TrieRecorder {
public:
	TrieRecorder() {}
	void addCommand(const TrieAnimationCommand& command) {
		phases.back().commands.push_back(command);
	}
	void addState(const TrieState& snapshot) {
		phases.back().snapshot = snapshot;
	}
	void addNewPhase() {
		phases.push_back(TrieAnimationPhase());
		highlighted_lines.push_back(-1);
	}
	void clear() {
		phases.clear();
		highlighted_lines.clear();
	}
	std::vector<TrieAnimationPhase> getPhases() const {
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
	std::vector<TrieAnimationPhase> phases;
	std::vector<int> highlighted_lines;
};