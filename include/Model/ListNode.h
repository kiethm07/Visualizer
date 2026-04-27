#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

class ListNode : public sf::Drawable {
public:
	explicit ListNode(const sf::Font& font, const std::string& label, const sf::Vector2f& pos, const int& size, const unsigned int char_size);
	void setPosition(const sf::Vector2f& pos);
	void setOrigin(const sf::Vector2f& origin);
	void setListNodeRadius(const int& size);
	void setLabel(const std::string& label);
	void setListNodeColor(const sf::Color& color);
	void setCharacterSize(const unsigned int& size);
	void setCharacterColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	bool contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position);

	sf::Vector2f getPosition();
	sf::Vector2f getOrigin();
	int getRadius();
	sf::FloatRect getGlobalBounds();
	std::string getLabel();
	bool isDragging() const {
		return is_dragging;
	}
protected:
	static inline const sf::Color IDLE_COLOR = sf::Color(89, 145, 46);
	static inline const sf::Color HOVER_COLOR = sf::Color(209, 70, 38);

	sf::CircleShape container;
	sf::Text text;
	sf::Vector2f drag_offset;

	bool hovered = 0;
	bool is_dragging = 0;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(container, states);
		target.draw(text, states);
	}

	void centerText();
};