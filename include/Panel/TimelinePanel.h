#pragma once
#include <SFML/Graphics.hpp>
#include <AssetManager/AssetManager.h>
#include <Model/Button.h>
#include <Model/TimelineConfig.h>
#include <optional>

class TimelinePanel : public sf::Drawable {
public:
	TimelinePanel(const AssetManager& a_manager);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(play, states);
		target.draw(auto_play, states);
		target.draw(one_phase_forward, states);
		target.draw(one_phase_backward, states);
		target.draw(one_step_forward, states);
		target.draw(one_step_backward, states);
		target.draw(skip_to_last, states);
		target.draw(skip_to_init, states);
	}
	std::optional<TimelineConfig> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
private:
	void updateWindowState(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	const AssetManager& a_manager;
	const sf::Font& FONT;
	sf::RectangleShape background;
	Button play, auto_play;
	Button one_phase_forward, one_phase_backward;
	Button one_step_forward, one_step_backward;
	Button skip_to_last, skip_to_init;
};