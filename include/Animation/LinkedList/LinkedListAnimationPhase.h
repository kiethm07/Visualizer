#pragma once
#include <vector>
#include <Animation/LinkedList/LinkedListAnimationCommand.h>

struct LinkedListAnimationPhase {
	std::vector<LinkedListAnimationCommand> commands;
};