#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <Model/GraphModel/GraphOperation.h>

struct GraphState {
	int next_ui_id = 0;
	struct Node {
		int ui_id;
		//connect value from 2 nodes
		//value is hidden in the key of the map
		//value is used to display the node
		std::map<int, int> neighbors;
		Node() {}
		Node(int id) :
			ui_id(id) {
		}
	};
	std::map<int, Node> nodes;
	std::map<std::pair<int, int>, int> edges;
};