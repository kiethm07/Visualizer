#pragma once
#include <Model/Button.h>
#include <Model/TextBox.h>
#include <Model/HashmapModel/HashmapOperation.h>
#include <optional>
#include <iostream>

class HashmapPanel : public sf::Drawable {
public:
	HashmapPanel(const sf::Font& BUTTON_FONT);

	sf::Vector2f getSize();
	void update(const sf::RenderWindow& window, const sf::View& view);
	std::optional<HashmapOperation> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);

private:
	const sf::Font& BUTTON_FONT;

	sf::RectangleShape background;

	TextBox input_value;

	Button insert_button;
	Button remove_button;
	Button search_button;
	Button reset_button;

	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void updateWindowState(const sf::RenderWindow& window, const sf::View& view);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(input_value, states);
		target.draw(insert_button, states);
		target.draw(remove_button, states);
		target.draw(search_button, states);
		target.draw(reset_button, states);
	}
};