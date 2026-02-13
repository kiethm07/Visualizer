#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button : public sf::Drawable {
public:
	Button(const sf::Font& font, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size);
	void setPosition(sf::Vector2f pos);
	void setSize(sf::Vector2f size);
	void setLabel(const std::string label);

private:
	sf::RectangleShape container;
	sf::Text text;
	bool hovered = 0;
	bool clicked = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(container, states);
		target.draw(text, states);
	}
	void centerText();
};