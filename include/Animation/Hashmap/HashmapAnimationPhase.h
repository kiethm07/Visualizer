#pragma once
#include <vector>
#include <Animation/Hashmap/HashmapAnimationCommand.h>

struct HashmapAnimationPhase {
	std::vector<HashmapAnimationCommand> commands;
};