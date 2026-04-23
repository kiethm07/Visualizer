#pragma once
#include <SFML/Graphics.hpp>
#include <Model/Button.h>
#include <Model/MenuState.h>
#include <optional>

class DSMenu : public sf::Drawable {
public:
	DSMenu(const sf::Font& font);
	std::optional<MenuState> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);
	void update(const sf::RenderWindow& window, const sf::View& view);
private:
	const sf::Font& menu_font;
	Button main_menu_button;
	Button linked_list_button;
	Button hashmap_button;
	Button trie_button;
	Button avl_button;
	Button graph_button;
	void updateWindowSize(const sf::RenderWindow& window, const sf::View& view);
	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(main_menu_button, states);
		target.draw(linked_list_button, states);
		target.draw(hashmap_button, states);
		target.draw(trie_button, states);
		target.draw(avl_button, states);
		target.draw(graph_button, states);
	}
};