#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Slider : public sf::Drawable {
public:
	Slider(const sf::Vector2f pos, const sf::Vector2f size, float min_val, float max_val, float initial_val);

	void setPosition(const sf::Vector2f& pos);
	void setOrigin(const sf::Vector2f& origin);
	void setSliderSize(const sf::Vector2f& size);
	void setValue(float val);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);

	bool contains(const sf::Vector2f& position) const;
	sf::Vector2f getPosition() const;
	sf::Vector2f getOrigin() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	float getValue() const;
	bool getIsDragging() const;

protected:
	static inline const sf::Color TRACK_COLOR = sf::Color(50, 50, 50);
	static inline const sf::Color FILL_COLOR = sf::Color(89, 145, 46);
	static inline const sf::Color HOVER_COLOR = sf::Color(209, 70, 38);
	static inline const sf::Color KNOB_COLOR = sf::Color::White;

	sf::RectangleShape track;
	sf::RectangleShape fill_track;
	sf::CircleShape knob;

	float min_value;
	float max_value;
	float current_value;
	bool is_dragging = 0;
	bool hovered = 0;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(track, states);
		target.draw(fill_track, states);
		target.draw(knob, states);
	}

	void updateVisuals();
};