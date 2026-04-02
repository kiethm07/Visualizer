#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/LinkedListPanel.h>
#include <Panel/TimelinePanel.h>
#include <DataStructures/LinkedList.h>
#include <Model/LinkedListModel/LinkedListTimeline.h>
#include <Model/LinkedListModel/LinkedListRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>

class LinkedListUI {
public:
	LinkedListUI(const AssetManager& a_manager);
	void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
	void draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
private:
	bool inAnimation = 0;
	const AssetManager& a_manager;
	const sf::Font& NODE_FONT;
	LinkedListPanel panel;
	LinkedListState current_state;
	LinkedListTimeline timeline;
	LinkedList list;
	LinkedListRecorder recorder;
	sf::Clock clock;
	Button test;
	TimelinePanel timeline_panel;
};  