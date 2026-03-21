#pragma once
#include <SFML/Graphics.hpp>
#include <Model/ListNode.h>
#include <Model/Arrow.h>
#include <AssetManager/AssetManager.h>
#include <Model/LinkedListModel/LinkedListState.h>
#include <Animation/LinkedList/LinkedListAnimationState.h>
#include <string>
#include <algorithm>

class LinkedListRenderer {
public:
	void loadState(const LinkedListAnimationState& animation_state);
	void draw(sf::RenderWindow& window, const sf::View& view);
	LinkedListRenderer(const AssetManager& asset_manager);
private:
	const AssetManager& a_manager;
	const int NODE_RADIUS = 50;
	const int NODE_GAP = 2 * NODE_RADIUS + 100;
	const int X_MARGIN = 400;
	const int Y_MARGIN = 400;
	std::vector<ListNode> node_list;
	std::vector<Arrow> edge_list;
};