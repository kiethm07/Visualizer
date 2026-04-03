#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/HashmapPanel.h>
#include <Panel/TimelinePanel.h>
#include <DataStructures/Hashmap.h>
#include <Model/HashmapModel/HashmapTimeline.h>
#include <Model/HashmapModel/HashmapRenderer.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>

class HashmapUI {
public:
	HashmapUI(const AssetManager& a_manager);
	void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
	void draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void setUIState(const UIState& state) {
		ui_state = state;
	}
private:
	bool inAnimation = 0;
	const AssetManager& a_manager;
	const sf::Font& NODE_FONT;
	HashmapPanel panel;
	HashmapState current_state;
	HashmapTimeline timeline;
	Hashmap hashmap;
	HashmapRecorder recorder;
	sf::Clock clock;
	TimelinePanel timeline_panel;
	UIState ui_state;
	int bucket_count = 10; //fixed until initial page is finished
};