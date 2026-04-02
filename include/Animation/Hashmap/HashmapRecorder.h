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
	}
	void clear() {
		phases.clear();
	}
	std::vector<HashmapAnimationPhase> getPhases() const {
		return phases;
	}
private:
	std::vector<HashmapAnimationPhase> phases;
};