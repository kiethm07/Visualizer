#pragma once
#include <vector>
#include <Animation/Trie/TrieAnimationCommand.h>

struct TrieAnimationPhase {
	std::vector<TrieAnimationCommand> commands;
};