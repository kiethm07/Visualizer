#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

//Save the state of the hashmap, allow reconstruct the hashmap using this state
struct HashmapState {
	int bucket_count;
	std::vector<std::vector<int>> value;
	std::vector<std::vector<int>> ui_id;
	int next_ui_id;
	HashmapState() :
		bucket_count(0),
		next_ui_id(0) {
	}
};