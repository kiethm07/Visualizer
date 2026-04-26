#pragma once
#include <SFML/Graphics.hpp>
#include <Model/ListNode.h>
#include <Model/GraphModel/GraphEdge.h>
#include <AssetManager/AssetManager.h>
#include <Model/GraphModel/GraphState.h>
#include <Model/GraphModel/GraphPhysics.h>
#include <Animation/Graph/GraphAnimationState.h>
#include <string>
#include <algorithm>

class GraphRenderer {
public:
	GraphRenderer(const AssetManager& asset_manager);
	void loadState(const GraphAnimationState& animation_state);
	void draw(sf::RenderWindow& window, const sf::View& view);
	void update(const sf::RenderWindow& window, const sf::View& view);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
private:
	GraphAnimationState current_state;
	const AssetManager& a_manager;
	std::vector<ListNode> node_list;
	std::vector<Arrow> edge_list;
	GraphPhysics physics;
	sf::Clock physics_clock;
	bool was_left_pressed = false;

	const int NODE_RADIUS = 50;
	const float SPEED_CONSTANT = 5.0;

	const sf::Color DEFAULT_WEIGHT_COLOR = sf::Color::Yellow;
	const sf::Color DEFAULT_POPUP_TEXT_COLOR = sf::Color::White;
};