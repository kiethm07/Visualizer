#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <AssetManager/AssetManager.h>
#include <Model/RoundedRectangleShape.h>
#include <UI/SoundController.h>
#include <string>
#include <iostream>
#include <cmath>

class Button : public sf::Drawable {
public:
	static void setSoundController(SoundController* controller) {
		sound_controller = controller;
	}
	explicit Button(const AssetManager& a_manager, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size);
	void setPosition(const sf::Vector2f& pos);
	void setOrigin(const sf::Vector2f& origin);
	void setButtonSize(const sf::Vector2f& size);
	void setLabel(const std::string& label);
	void setCharacterSize(const unsigned int& size);
	void setCharacterColor(const sf::Color& color);
	void setOutlineColor(const sf::Color& color);
	void setCornerRadius(float radius);
	void setPointCount(int point_count);
	bool handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	bool contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position);
	void setFont(const std::string& font_name);
	void setFont(const sf::Font& font);

	sf::Vector2f getPosition() const;
	sf::Vector2f getCenter() const;
	sf::Vector2f getOrigin() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	std::string getLabel() const;
	bool getHover() const {
		return hovered;
	}
protected:
	static inline const std::string DEFAULT_FONT_NAME = "Roboto-Regular";
	static inline const sf::Color IDLE_COLOR = sf::Color(45, 45, 50, 230);
	static inline const sf::Color HOVER_COLOR = sf::Color(60, 60, 70, 255);
	static inline const sf::Color NEON_CYAN = sf::Color(0, 255, 255);
	static inline const sf::Color TEXT_IDLE = sf::Color(200, 200, 200);
	static inline const float SOUND_FACTOR = 0.8f;

	const AssetManager& a_manager;
	RoundedRectangleShape container;
	sf::Text text;
	sf::Sound click_sound;
	//sf::Sound hover_sound;
	static inline SoundController* sound_controller = nullptr;

	bool hovered = 0;
	float currentScale = 1.0f;
	float targetScale = 1.0f;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(container, states);
		target.draw(text, states);
	}

	//const sf::Color IDLE_COLOR = sf::Color(89, 145, 46);
	//const sf::Color HOVER_COLOR = sf::Color(209, 70, 38);
	void centerText();
};