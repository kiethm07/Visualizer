#include <DSUI/LinkedListUI.h>
#include <iostream>

LinkedListUI::LinkedListUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	test(NODE_FONT, "TEST", {}, {}, 30),
	renderer(a_manager)
{
	test.setButtonSize({ 200.f,200.f });
}

void LinkedListUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	panel.update(window, fixed_view);
	test.setPosition(panel.getSize() + sf::Vector2f({ 100.f, -300.f }));
	test.update(window, cam_view);
	//renderer.update(window, cam_view);
}

void LinkedListUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	auto op = panel.handleEvent(window, view, ev);
	if (op.has_value()) {
		timeline.push(current_state, *op);
		list.applyOperation(*op);
		current_state = list.getState();
		renderer.loadState(current_state);
	}
}

void LinkedListUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	window.setView(fixed_view);
	window.draw(panel);
	window.setView(cam_view);
	//window.draw(test);
	renderer.draw(window, cam_view);
}