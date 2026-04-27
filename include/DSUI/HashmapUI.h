#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/HashmapPanel.h>
#include <Panel/TimelinePanel.h>
#include <Panel/InitPanel.h>
#include <DataStructures/Hashmap.h>
#include <Model/HashmapModel/HashmapTimeline.h>
#include <Model/HashmapModel/HashmapRenderer.h>
#include <Model/HashmapModel/HashmapDictionary.h>
#include <Model/PseudoCodeController/PseudoCodeController.h>
#include <Model/Button.h>
#include <AssetManager/AssetManager.h>
#include <UI/Camera.h>
#include <DSUI/UIState.h>
#include <Model/MenuState.h>
#include <chrono>
#include <random>

class HashmapUI {
public:
	HashmapUI(const AssetManager& a_manager);
	void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev);
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
	HashmapPanel panel;
	HashmapState current_state;
	HashmapTimeline timeline;
	Hashmap hashmap;
	HashmapRecorder recorder;
	sf::Clock clock;
	TimelinePanel timeline_panel;
	UIState ui_state;
	InitPanel init_panel;
	PseudoCodeController<HashmapOperationType, HashmapDictionary> code_panel;
	const int DEFAULT_BUCKET_COUNT = 13;
	sf::Sprite background;
};