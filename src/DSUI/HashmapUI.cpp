#include <DSUI/HashmapUI.h>
#include <iostream>

HashmapUI::HashmapUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	timeline(a_manager),
	timeline_panel(a_manager){
}

void HashmapUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	panel.update(window, fixed_view);
	timeline_panel.update(window, fixed_view);
	//timeline.update(clock.restart().asSeconds());
}

void HashmapUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, sf::View& cam_view, CameraController& cam, const sf::Event& ev) {
}

void HashmapUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	window.setView(cam_view);
	//timeline.draw(window, cam_view);
	window.setView(fixed_view);
	window.draw(panel);
	window.draw(timeline_panel);
}