#pragma once
#include <SFML/Graphics.hpp>
#include <UI/Camera.h>
#include <UI/DSMenu.h>
#include <UI/MainMenu.h>
#include <optional>
#include <Model/MenuState.h>
#include <AssetManager/AssetManager.h>
#include <DSUI/LinkedListUI.h>
#include <DSUI/HashmapUI.h>

class UI{
public:
	UI(const AssetManager& asset_manager);
	void run();
private:
	void update();
	void handleEvent(const sf::Event& ev);
	const AssetManager& asset_manager;
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::View cam_view, fixed_view;
	CameraController cam;
	MainMenu main_menu;
	DSMenu ds_menu;
	LinkedListUI linked_list_ui;
	HashmapUI hashmap_ui;
	MenuState current_state = MenuState::MainMenu;
	bool changed_state = 1;
	const unsigned int DEFAULT_WIDTH = 1280;
	const unsigned int DEFAULT_HEIGHT = 720;

	const unsigned int MIN_WIDTH = 800;
	const unsigned int MIN_HEIGHT = 600;
};