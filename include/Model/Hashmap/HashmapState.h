#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

//Save the state of the hashmap, allow reconstruct the hashmap using this state
struct HashmapState {
	int bucket_count;
	int next_ui_id;
	std::vector<int> key;
	std::vector<int> value;
	std::vector<int> ui_id;
	std::vector<int> bucket_index;
	int getSize() {
		return key.size();
	}
	HashmapState() :
		bucket_count(0),
		next_ui_id(0) {
	}
};