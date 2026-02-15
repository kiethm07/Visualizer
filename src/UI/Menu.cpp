#include <UI/Menu.h>
#include <iostream>

Menu::Menu(const sf::Font& font) :
	menu_font(font),
	start_button(font, "Start", {}, {}, 20),
	setting_button(font, "", {}, {}, 20) {}

void Menu::update(const sf::RenderWindow& window, const sf::View& view) {
	updateWindowSize(window, view);
	updateButtonState(window, view);
}

std::optional<MenuState> Menu::handleEvent(const sf::RenderWindow & window, const sf::Event & ev) {
	
	return std::nullopt;
}

void Menu::updateWindowSize(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();
	sf::Vector2f center = { size.x / 2.f, size.y / 2.f };
	//std::cout << center.x << " " << center.y << "\n";
	float button_width = 0.2f * size.x;
	float button_height = 0.1f * size.y;
	unsigned int button_text_size = 25;
	start_button.setButtonSize({ button_width, button_height });
	start_button.setOrigin({ button_width / 2, button_height / 2 });
	start_button.setPosition(center);
	start_button.setCharacterSize(button_text_size);
	//std::cout << start_button.getPosition().x << " " << start_button.getPosition().y << "\n";
	//std::cout << start_button.getOrigin().x << " " << start_button.getOrigin().y << "\n";
	//std::cout << button_width << " " << button_height << "\n";
}

void Menu::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	start_button.update(window, view);
}