#pragma once
#include <SFML/Graphics.hpp>
#include <Model/ListNode.h>
#include <Model/Arrow.h>
#include <AssetManager/AssetManager.h>
#include <Model/HashmapModel/HashmapState.h>
#include <Animation/Hashmap/HashmapAnimationState.h>
#include <string>
#include <algorithm>

class HashmapRenderer {
public:
	HashmapRenderer(const AssetManager& asset_manager);
	void loadState(const HashmapAnimationState& animation_state);
	void draw(sf::RenderWindow& window, const sf::View& view);
private:
	const AssetManager& a_manager;

	std::vector<ListNode> node_list;
	std::vector<Arrow> edge_list;
	std::vector<sf::RectangleShape> bucket_shape_list;
	std::vector<sf::Text> bucket_text_list;
};