#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class CodeLine : public sf::Drawable {
public:
	CodeLine(const std::string& label, const sf::Font& font, unsigned int char_size);
	void setPosition(const sf::Vector2f& pos);
	void setBackgroundColor(const sf::Color& color);
	void setTextColor(const sf::Color& color);
	void setSize(const sf::Vector2f& size);
	sf::Vector2f getPosition() const {
		return background.getPosition();
	}
	sf::Vector2f getSize() const {
		return background.getSize();
	}
private:
	static inline const int X_PADDING = 5;
	static inline const sf::Color DEFAULT_BG_COLOR = sf::Color(255, 255, 255);
	static inline const sf::Color DEFAULT_TEXT_COLOR = sf::Color(0, 0, 0);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Text text;
	sf::RectangleShape background;
};