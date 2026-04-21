#pragma once
#include <Model/Button.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Button::Button(const sf::Font& font, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size) : text(font, label, char_size) {
	container.setPosition(pos);
	container.setSize(size);
	container.setFillColor(IDLE_COLOR);
	container.setOutlineThickness(1);
	container.setOutlineColor(sf::Color::White);
	centerText();
}

void Button::setOutlineColor(const sf::Color& color) {
	container.setOutlineColor(color);
}

void Button::setPosition(const sf::Vector2f& pos) {
	container.setPosition(pos);
	centerText();
}

sf::Vector2f Button::getPosition() const {
	return container.getPosition();
}

void Button::setButtonSize(const sf::Vector2f& size) {
	container.setSize(size);
	centerText();
}

sf::Vector2f Button::getCenter() const {
	sf::Vector2f upper_left = container.getPosition() - container.getOrigin();
	return upper_left + container.getSize() / 2.f;
}

sf::Vector2f Button::getSize() const {
	return container.getSize();
}

void Button::setOrigin(const sf::Vector2f& origin) {
	container.setOrigin(origin);
	centerText();
}

sf::Vector2f Button::getOrigin() const {
	return container.getOrigin();
}

void Button::setLabel(const std::string& label) {
	text.setString(label);
	centerText();
}

void Button::setCharacterSize(const unsigned int& size) {
	text.setCharacterSize(size);
	centerText();
}

void Button::setCharacterColor(const sf::Color& color) {
	text.setFillColor(color);
}

std::string Button::getLabel() const {
	return text.getString();
}

sf::FloatRect Button::getGlobalBounds() const {
	return container.getGlobalBounds();
}

bool Button::contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	return container.getGlobalBounds().contains(mouse_world);
}

void Button::update(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	hovered = container.getGlobalBounds().contains(mouse_world);
	container.setFillColor(hovered ? HOVER_COLOR : IDLE_COLOR);
}

bool Button::mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (const auto* mouse_press = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_press->button == sf::Mouse::Button::Left) {
			sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			return container.getGlobalBounds().contains(mouse_world);
		}
	}
	return 0;
}

void Button::centerText() {
	sf::FloatRect box = container.getGlobalBounds();
	sf::Vector2f center = box.position + box.size / 2.f;

	auto b = text.getLocalBounds();
	//text.setOrigin(b.position + b.size / 2.f);
	//text.setPosition(center);

	sf::Vector2f text_origin = b.position + b.size / 2.f;
	text_origin.x = lround(text_origin.x);
	text_origin.y = lround(text_origin.y);
	center.x = lround(center.x);
	center.y = lround(center.y);

	text.setOrigin(text_origin);
	text.setPosition(center);
}