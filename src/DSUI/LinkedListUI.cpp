#include <DSUI/LinkedListUI.h>
#include <iostream>

LinkedListUI::LinkedListUI(const sf::Font& NODE_FONT) :
	NODE_FONT(NODE_FONT),
	panel(NODE_FONT),
	test(NODE_FONT, "TEST", {}, {}, 30)
{
	test.setButtonSize({ 200.f,200.f });
}

void LinkedListUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	panel.update(window, fixed_view);
	test.setPosition(panel.getSize() + sf::Vector2f({ 100.f, -300.f }));
	test.update(window, cam_view);
}

void LinkedListUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	panel.handleEvent(window, view, ev);
}

void LinkedListUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	window.setView(fixed_view);
	window.draw(panel);
	window.setView(cam_view);
	window.draw(test);
}