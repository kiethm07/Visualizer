#pragma once
#include <UI/SoundController.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <AssetManager/AssetManager.h>
#include <string>
#include <cmath>

class SoundController;

class TextureButton : public sf::Drawable {
public:
	static void setSoundController(SoundController* controller) {
		if (!sound_controller) {
			sound_controller = controller;
		}
	}

	explicit TextureButton(const AssetManager& a_manager, const std::string& texture_name, const sf::Vector2f pos);

	void setPosition(const sf::Vector2f& pos);
	sf::Vector2f getPosition() const;

	void setOrigin(const sf::Vector2f& origin);
	sf::Vector2f getOrigin() const;

	void setScale(const sf::Vector2f& scale);
	void setButtonSize(const sf::Vector2f& size);
	void setTexture(const std::string& texture_name);
	sf::FloatRect getGlobalBounds() const;
	bool contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position);

	bool handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	bool mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);

protected:
	static inline const float SOUND_FACTOR = 1.0f;

	const AssetManager& a_manager;
	sf::Sprite sprite;
	sf::Sound click_sound;

	bool hovered = 0;
	float currentScale = 1.0f;
	float targetScale = 1.0f;
	sf::Vector2f baseScale = { 1.0f, 1.0f };
	sf::Vector2f lastTargetSize = { 0.f, 0.f };

	static SoundController* sound_controller;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(sprite, states);
	}
};