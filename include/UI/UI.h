#pragma once
#include <SFML/Graphics.hpp>
#include <UI/Camera.h>
#include <UI/Menu.h>
#include <optional>
#include <Model/MenuState.h>
#include <AssetManager/AssetManager.h>

class UI{
public:
	UI(const AssetManager& asset_manager);
	void run();
private:
	const AssetManager& asset_manager;
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::View view;
	CameraController cam;
	Menu menu;
	MenuState current_state = MenuState::Menu;
	const unsigned int DEFAULT_WIDTH = 1280;
	const unsigned int DEFAULT_HEIGHT = 720;

	const unsigned int MIN_WIDTH = 800;
	const unsigned int MIN_HEIGHT = 600;
};