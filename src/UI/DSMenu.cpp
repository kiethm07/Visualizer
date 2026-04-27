#include <UI/DSMenu.h>
#include <iostream>

DSMenu::DSMenu(const AssetManager& a_manager) :
	a_manager(a_manager),
	menu_font(a_manager.getFont("Silkscreen-Regular")),
	background(a_manager.getTexture("MinecraftBackground")),
	main_menu_button(menu_font, "Main Menu", {}, {}, 20),
	linked_list_button(menu_font, "Linked List", {}, {}, 20),
	hashmap_button(menu_font, "Hash Map", {}, {}, 20),
	trie_button(menu_font, "Trie", {}, {}, 20),
	avl_button(menu_font, "AVL Tree", {}, {}, 20),
	graph_button(menu_font, "Graph", {}, {}, 20) {
}

void DSMenu::update(const sf::RenderWindow& window, const sf::View& view) {
	updateWindowSize(window, view);
	updateButtonState(window, view);
}

std::optional<MenuState> DSMenu::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (ev.getIf<sf::Event::Resized>()) {
		updateWindowSize(window, view);
	}

	if (const auto* mouse = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse->button == sf::Mouse::Button::Left) {
			if (main_menu_button.handleEvent(window, view, ev)) return MenuState::MainMenu;
			if (linked_list_button.handleEvent(window, view, ev)) return MenuState::LinkedList;
			if (hashmap_button.handleEvent(window, view, ev)) return MenuState::Hashmap;
			if (trie_button.handleEvent(window, view, ev)) return MenuState::Trie;
			if (avl_button.handleEvent(window, view, ev)) return MenuState::AVL;
			if (graph_button.handleEvent(window, view, ev)) return MenuState::Graph;
		}
	}

	return std::nullopt;
}

void DSMenu::updateWindowSize(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();

	float button_width = 0.2f * size.x;
	float button_height = 0.08f * size.y;
	unsigned int text_size = 25;

	sf::Vector2f button_size = { button_width, button_height };
	sf::Vector2f origin = { button_width / 2.f, button_height / 2.f };

	float start_y = size.y * 0.2f;
	float gap = button_height * 1.5f;

	auto place_button = [&](Button& btn, float index) {
		btn.setButtonSize(button_size);
		btn.setOrigin(origin);
		btn.setPosition({ size.x / 2.f, start_y + index * gap });
		btn.setCharacterSize(text_size);
		};

	place_button(main_menu_button, 0);
	place_button(linked_list_button, 1);
	place_button(hashmap_button, 2);
	place_button(trie_button, 3);
	place_button(avl_button, 4);
	place_button(graph_button, 5);

	sf::Vector2u windowSize = window.getSize();

	float scaleX = (float)windowSize.x / background.getTexture().getSize().x;
	float scaleY = (float)windowSize.y / background.getTexture().getSize().y;

	background.setScale({ scaleX, scaleY });
}

void DSMenu::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	main_menu_button.update(window, view);
	linked_list_button.update(window, view);
	hashmap_button.update(window, view);
	trie_button.update(window, view);
	avl_button.update(window, view);
	graph_button.update(window, view);
}