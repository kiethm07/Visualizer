#pragma once
#include <SFML/Graphics.hpp>
#include <AssetManager/AssetManager.h>
#include <Model/MenuState.h>
#include <Model/Button.h>
#include <Model/Slider.h>
#include <Model/TextureButton.h>
#include <UI/SoundController.h>
#include <optional>
#include <External/SimpleFileDialog.h>

class Setting : public sf::Drawable {
public:
	static void setSoundController(SoundController* controller) {
		sound_controller = controller;
	}
	Setting(const AssetManager& a_manager);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
	void setPreviouState(const MenuState& state) {
		previous_state = state;
	}
private:
	const AssetManager& a_manager;
	static inline SoundController* sound_controller = nullptr;

	sf::Sprite background;
	RoundedRectangleShape subbackground;
	MenuState previous_state = MenuState::Setting;

	TextureButton icon_music;
	Slider slider_music;

	TextureButton icon_sfx;
	Slider slider_sfx;

	Button btn_choose_music;
	Button btn_back;

	bool music_mute = 0;
	bool sfx_mute = 0;

	void updateWindowSize(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(subbackground, states);
		target.draw(icon_music, states);
		target.draw(slider_music, states);
		target.draw(icon_sfx, states);
		target.draw(slider_sfx, states);
		target.draw(btn_choose_music, states);
		target.draw(btn_back, states);
	}
};