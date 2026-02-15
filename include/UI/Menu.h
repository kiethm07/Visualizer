#pragma once
#include <SFML/Graphics.hpp>
#include <Model/Button.h>
#include <Model/MenuState.h>
#include <optional>

class Menu : public sf::Drawable{
public:
	Menu(const sf::Font& font);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::Event& ev);
	void update(const sf::RenderWindow& window);
private:
	const sf::Font& menu_font;
	Button start_button;
	Button setting_button;
	void updateWindowSize(const sf::Vector2u& size);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(start_button, states);
		target.draw(setting_button, states);
	}
};