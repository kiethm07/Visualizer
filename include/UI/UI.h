#pragma once
#include <SFML/Graphics.hpp>
#include <UI/Camera.h>
#include <optional>
#include <Model/MenuState.h>

class UI{
public:
	UI();
	void run();
private:
	sf::RenderWindow window;
	sf::ContextSettings settings;
	sf::View view;
	CameraController cam;
	MenuState current_state = MenuState::Menu;
	const unsigned int DEFAULT_WIDTH = 1280;
	const unsigned int DEFAULT_HEIGHT = 720;

	const unsigned int MIN_WIDTH = 800;
	const unsigned int MIN_HEIGHT = 600;
};