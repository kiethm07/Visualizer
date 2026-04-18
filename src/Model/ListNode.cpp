#pragma once
#include <Model/ListNode.h>
#include <SFML/Graphics.hpp>
#include <iostream>

ListNode::ListNode(const sf::Font& font, const std::string& label, const sf::Vector2f& pos, const int& size, const unsigned int char_size) : text(font, label, char_size) {
	container.setRadius(size);
	container.setOrigin(sf::Vector2f{ (float)size, (float)size });
	container.setPosition(pos);
	container.setFillColor(IDLE_COLOR);
	container.setPointCount(75);
	centerText();
}

void ListNode::setPosition(const sf::Vector2f& pos) {
	container.setPosition(pos);
	centerText();
}

sf::Vector2f ListNode::getPosition() {
	return container.getPosition();
}

void ListNode::setListNodeRadius(const int& size) {
	container.setRadius(size);
	centerText();
}

void ListNode::setListNodeColor(const sf::Color& color) {
	container.setFillColor(color);
}

int ListNode::getRadius() {
	return container.getRadius();
}

void ListNode::setOrigin(const sf::Vector2f& origin) {
	container.setOrigin(origin);
	centerText();
}

sf::Vector2f ListNode::getOrigin() {
	return container.getOrigin();
}

void ListNode::setLabel(const std::string& label) {
	text.setString(label);
	centerText();
}

void ListNode::setCharacterSize(const unsigned int& size) {
	text.setCharacterSize(size);
	centerText();
}

void ListNode::setCharacterColor(const sf::Color& color) {
	text.setFillColor(color);
}

std::string ListNode::getLabel() {
	return text.getString();
}

sf::FloatRect ListNode::getGlobalBounds() {
	return container.getGlobalBounds();
}

bool ListNode::contains(const sf::RenderWindow& window, const sf::View& view, const sf::Vector2f& position) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	return container.getGlobalBounds().contains(mouse_world);
}

void ListNode::update(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	hovered = container.getGlobalBounds().contains(mouse_world);
	container.setFillColor(hovered ? HOVER_COLOR : IDLE_COLOR);
}

bool ListNode::mousePressed(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (const auto* mouse_press = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_press->button == sf::Mouse::Button::Left) {
			sf::Vector2f mouse_world = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			return container.getGlobalBounds().contains(mouse_world);
		}
	}
	return 0;
}

void ListNode::centerText() {
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