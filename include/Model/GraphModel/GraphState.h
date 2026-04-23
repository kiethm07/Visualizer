#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Model/GraphModel/GraphOperation.h>

struct GraphNodeSnapshot {
	int value;
	int ui_id;
	int size;
	int height;
	int leftChild = -1;
	int rightChild = -1;
	int parent = -1;
};

//Save the state of the trie, allow reconstruct the trie using this state
struct GraphState {
	std::vector<GraphNodeSnapshot> nodes;
	int next_ui_id;
	GraphState() {
		next_ui_id = 0;
	}
};
