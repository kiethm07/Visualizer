#pragma once
#include <SFML/Graphics.hpp>

class LinkedListUI : public sf::Drawable{
public:
	LinkedListUI(const sf::Font& NODE_FONT);
private:
	const sf::Font& NODE_FONT;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		const sf::View current_view = target.getView();
	}
};