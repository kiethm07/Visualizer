#pragma once
#include <SFML/Graphics.hpp>
#include <Model/Button.h>
#include <Model/LinkedListModel/ListOperation.h>
#include <optional>

class LinkedListPanel : public sf::Drawable {
public:
	LinkedListPanel(const sf::Font& BUTTON_FONT);
	void update(const sf::RenderWindow& window, const sf::View& view);
	std::optional<ListOperation> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);

	sf::Vector2f getSize();
private:
	const sf::Font& BUTTON_FONT;
	Button reset_button;
	Button insert_last_button;
	Button remove_last_button;
	sf::RectangleShape background;
	void updateWindowState(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		const sf::View current_view = target.getView();
		target.draw(background, states);
		target.draw(reset_button, states);
		target.draw(insert_last_button, states);
		target.draw(remove_last_button, states);
	}
};