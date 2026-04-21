#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/LinkedListPanel.h>
#include <Panel/TimelinePanel.h>
#include <Panel/InitPanel.h>
#include <Panel/PseudoCodePanel.h>
#include <DataStructures/LinkedList.h>
#include <Model/LinkedListModel/LinkedListTimeline.h>
#include <Model/LinkedListModel/LinkedListRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>
#include <chrono>
#include <random>

class LinkedListUI {
public:
	LinkedListUI(const AssetManager& a_manager);
	void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
	void draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void setInit() {
		ui_state = UIState::Init;
		init_panel.clearValueForManualInput();
	}
private:
	void Init(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const PanelData& data);
	bool inAnimation = 0;
	const AssetManager& a_manager;
	const sf::Font& NODE_FONT;
	LinkedListPanel panel;
	InitPanel init_panel;
	TimelinePanel timeline_panel;
	LinkedListTimeline timeline;
	LinkedListRecorder recorder;
	sf::Clock clock;
	LinkedListState current_state;
	UIState ui_state;
	LinkedList list;
	Button test;

	PseudoCodePanel code_panel;
};  