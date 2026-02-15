#pragma once
#include <Model/Button.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Button::Button(const sf::Font& font, const std::string& label, const sf::Vector2f pos, const sf::Vector2f size, const unsigned int char_size) : text(font, label, char_size) {
	container.setPosition(pos);
	container.setSize(size);
	container.setFillColor(IDLE_COLOR);
	
	centerText();
}

void Button::setPosition(const sf::Vector2f pos) {
	container.setPosition(pos);
	text.setPosition(pos);
	centerText();
}

sf::Vector2f Button::getPosition() {
	return container.getPosition();
}

void Button::setSize(const sf::Vector2f size) {
	container.setSize(size);
	centerText();
}

sf::Vector2f Button::getSize() {
	return container.getSize();
}

void Button::setOrigin(const sf::Vector2f origin) {
	container.setOrigin(origin);
	text.setOrigin(origin);
	centerText();
}

sf::Vector2f Button::getOrigin() {
	return container.getOrigin();
}

void Button::setLabel(const std::string label) {
	text.setString(label);
	centerText();
}

std::string Button::getLabel() {
	return text.getString();
}

sf::FloatRect Button::getGlobalBounds() {
	return container.getGlobalBounds();
}

void Button::update(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	hovered = container.getGlobalBounds().contains(mouse_world);
	container.setFillColor(hovered ? HOVER_COLOR : IDLE_COLOR);
}

void Button::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	std::cout << hovered << "\n";
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	hovered = container.getGlobalBounds().contains(mouse_world);
	if (hovered == 0) container.setFillColor(IDLE_COLOR);
	if (hovered == 1) container.setFillColor(HOVER_COLOR);
}

void Button::centerText() {
	auto box = container.getGlobalBounds();
	sf::Vector2f center = box.position + box.size / 2.f;

	auto b = text.getLocalBounds();
	text.setOrigin(b.position + b.size / 2.f);
	text.setPosition(center);
}