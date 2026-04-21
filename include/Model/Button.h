#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button : public sf::Drawable {
public:
	explicit Button(const sf::Font& font, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size);
	void setPosition(const sf::Vector2f& pos);
	void setOrigin(const sf::Vector2f& origin);
	void setButtonSize(const sf::Vector2f& size);
	void setLabel(const std::string& label);
	void setCharacterSize(const unsigned int& size);
	void setCharacterColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	//void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	bool contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position);

	sf::Vector2f getPosition() const;
	sf::Vector2f getCenter() const;
	sf::Vector2f getOrigin() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	std::string getLabel() const;
protected:
	sf::RectangleShape container;
	sf::Text text;
	bool hovered = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(container, states);
		target.draw(text, states);
	}

	const sf::Color IDLE_COLOR = sf::Color(89, 145, 46);
	const sf::Color HOVER_COLOR = sf::Color(209, 70, 38);

	void centerText();
};