#pragma once
#include <SFML/Graphics.hpp>
#include <Model/Button.h>
#include <Model/MenuState.h>
#include <optional>

class MainMenu : public sf::Drawable {
public:
	MainMenu(const sf::Font& font);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void MainMenu::update(const sf::RenderWindow& window, const sf::View& view);
private:
	const sf::Font& menu_font;
	Button start_button;
	Button setting_button;
	void updateWindowSize(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(start_button, states);
		target.draw(setting_button, states);
	}
};