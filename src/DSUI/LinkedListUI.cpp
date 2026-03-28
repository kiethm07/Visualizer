#include <DSUI/LinkedListUI.h>
#include <iostream>

LinkedListUI::LinkedListUI(const AssetManager& a_manager) :
	a_manager(a_manager),
	NODE_FONT(a_manager.getFont("Roboto-Regular")),
	panel(NODE_FONT),
	test(NODE_FONT, "TEST", {}, {}, 30),
	timeline(a_manager),
	timeline_panel(a_manager)
{
	test.setButtonSize({ 200.f,200.f });
}

void LinkedListUI::update(const sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	panel.update(window, fixed_view);
	timeline_panel.update(window, fixed_view);
	//test.setPosition(panel.getSize() + sf::Vector2f({ 100.f, -300.f }));
	//test.update(window, cam_view);
	//renderer.update(window, cam_view);
	timeline.update(window, cam_view, clock.restart().asSeconds());
}

void LinkedListUI::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	auto op = panel.handleEvent(window, view, ev);
	if (op.has_value()) {
		recorder.clear();
		list.applyOperation(*op, recorder);
		timeline.push(current_state, *op, recorder);
		current_state = list.getState(); 
	}
	if (const auto* key = ev.getIf<sf::Event::KeyPressed>()) {
		if (key->code == sf::Keyboard::Key::Space) {
			if (timeline.isRunning()) {
				timeline.pause();
				std::cout << "timeline paused\n";
			}
			else {
				timeline.run();
				std::cout << "timeline running\n";
			}
		}
		if (key->code == sf::Keyboard::Key::Right) {
			timeline.setDirection(1);
		}
		if (key->code == sf::Keyboard::Key::Left) {
			timeline.setDirection(-1);
		}
	}
}

void LinkedListUI::draw(sf::RenderWindow& window, const sf::View& fixed_view, const sf::View& cam_view) {
	window.setView(cam_view);
	//window.draw(test);
	timeline.draw(window, cam_view);
	window.setView(fixed_view);
	window.draw(panel);
	window.draw(timeline_panel);
}
