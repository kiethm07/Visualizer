#pragma once
#include <vector>
#include <Animation/Trie/TrieAnimationCommand.h>
#include <Model/TrieModel/TrieState.h>
#include <optional>

struct TrieAnimationPhase {
	std::vector<TrieAnimationCommand> commands;
	std::optional<TrieState> snapshot; //Use for reconstruct
};