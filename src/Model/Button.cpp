#pragma once
#include <Model/Button.h>
#include <SFML/Graphics.hpp>

Button::Button(const sf::Font& font, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size) : text(font, label, char_size) {
	container.setPosition(pos);
	container.setSize(size);
	container.setFillColor(sf::Color::Green);

	centerText();
}

void Button::centerText() {
	auto box = container.getGlobalBounds();
	sf::Vector2f center = box.position + box.size / 2.f;

	auto b = text.getLocalBounds();
	text.setOrigin(b.position + b.size / 2.f);
	text.setPosition(center);
}