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
	}
	void clear() {
		phases.clear();
	}
	std::vector<TrieAnimationPhase> getPhases() const {
		return phases;
	}
private:
	std::vector<TrieAnimationPhase> phases;
};