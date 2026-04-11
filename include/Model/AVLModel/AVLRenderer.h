#pragma once
#include <SFML/Graphics.hpp>
#include <Model/ListNode.h>
#include <Model/Arrow.h>
#include <AssetManager/AssetManager.h>
#include <Model/AVLModel/AVLState.h>
#include <Animation/AVL/AVLAnimationState.h>
#include <string>
#include <algorithm>

class AVLRenderer {
public:
	AVLRenderer(const AssetManager& asset_manager);
	void loadState(const AVLAnimationState& animation_state);
	void draw(sf::RenderWindow& window, const sf::View& view);
private:
	const AssetManager& a_manager;
	std::vector<ListNode> node_list;
	std::vector<Arrow> edge_list;
	const int NODE_RADIUS = 50;

};