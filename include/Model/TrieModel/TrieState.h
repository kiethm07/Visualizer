#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Model/TrieModel/TrieOperation.h>

struct TrieNodeSnapshot {
	std::string label;
	int ui_id;
	int cnt;
	int child[26];
	bool isEnd;
	TrieNodeSnapshot() {
		for (int i = 0; i < 26; i++) child[i] = -1;
	}
};

//Save the state of the trie, allow reconstruct the trie using this state
struct TrieState {
	std::vector<TrieNodeSnapshot> nodes;
	int next_ui_id;
	TrieState() {
		next_ui_id = 0;
	}
};
