#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <Model/LinkedListModel/ListOperation.h>

//Save the state of the linked list, allow reconstruct the linked list using this state
struct LinkedListState {
	int next_ui_id;
	std::vector<int> value;
	std::vector<int> ui_id;
	LinkedListState() :
		next_ui_id(0) {}
	void apply(const ListOperation& operation) {

	}
};
