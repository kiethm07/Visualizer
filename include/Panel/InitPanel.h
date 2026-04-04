#pragma once
#include <SFML/Graphics.hpp>
#include <AssetManager/AssetManager.h>
#include <Model/Button.h>
#include <Model/TextBox.h>
#include <Panel/PanelData.h>
#include <External/SimpleFileDialog.h>
#include <optional>
#include <iostream>
#include <fstream>

class InitPanel : public sf::Drawable {
public:
	InitPanel(const AssetManager& a_manager);
	void update(const sf::RenderWindow& window, const sf::View& view);
	std::optional<PanelData> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void setPlaceHolderForManualInput(const std::string& s);
	void clearValueForManualInput() {
		manual_input.setValue("");
	}
private:
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void updateWindowState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(manual_input, states);
		target.draw(random, states);
		target.draw(empty, states);
		target.draw(from_file, states);
	}
	const AssetManager& a_manager;
	const sf::Font& FONT;
	sf::RectangleShape background;
	TextBox manual_input;
	Button random;
	Button empty;
	Button from_file;
};