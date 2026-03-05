#pragma once
#include <SFML/Graphics.hpp>
#include <Panel/LinkedListPanel.h>
#include <DataStructures/LinkedList.h>
#include <Model/Button.h>

class LinkedListUI {
public:
	LinkedListUI(const sf::Font& NODE_FONT);
	void update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view);
private:
	const sf::Font& NODE_FONT;
	LinkedListPanel panel;
	LinkedList list;
	Button test;
};