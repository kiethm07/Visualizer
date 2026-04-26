#pragma once
#include <SFML/Graphics.hpp>
#include <Model/Button.h>
#include <Model/MenuState.h>
#include <AssetManager/AssetManager.h>
#include <optional>

class MainMenu : public sf::Drawable {
public:
	MainMenu(const AssetManager& a_manager);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
private:
	const AssetManager& a_manager;
	const sf::Font& menu_font;
	sf::Sprite background;
	sf::Sprite title;
	Button start_button;
	Button setting_button;
	void updateWindowSize(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(title, states);
		target.draw(start_button, states);
		target.draw(setting_button, states);
	}
};