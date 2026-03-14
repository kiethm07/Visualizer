#pragma once
#include <SFML/Graphics.hpp>
#include <Model/ListNode.h>
#include <AssetManager/AssetManager.h>
#include <Model/LinkedListModel/LinkedListState.h>
#include <string>

class LinkedListRenderer {
public:
	void loadState(const LinkedListState& list_state);
	void draw(sf::RenderWindow& window, const sf::View& view);
	LinkedListRenderer(const AssetManager& asset_manager);
private:
	const AssetManager& a_manager;
	int node_size = 50;
	int node_gap = 150;
	const int X_MARGIN = 400;
	const int Y_MARGIN = 400;
	std::vector<ListNode> vec_list;
};