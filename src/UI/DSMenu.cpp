#include <UI/DSMenu.h>
#include <iostream>

DSMenu::DSMenu(const sf::Font& font) :
	menu_font(font),
	main_menu_button(font, "Main Menu", {}, {}, 20),
	linked_list_button(font, "Linked List", {}, {}, 20) {
}

void DSMenu::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowSize(window, view);
}

std::optional<MenuState> DSMenu::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	if (const auto* resized = ev.getIf<sf::Event::Resized>()) {
		updateWindowSize(window, view);
	}
	if (const auto* mouse_pressed = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_pressed->button == sf::Mouse::Button::Left) {
			if (main_menu_button.mousePressed(window, view, ev)) {
				return MenuState::MainMenu;
			}
			if (linked_list_button.mousePressed(window, view, ev)) {
				return MenuState::LinkedList;
			}
		}
	}
	return std::nullopt;
}

void DSMenu::updateWindowSize(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();
	sf::Vector2f center = { size.x / 2.f, size.y / 2.f };
	//std::cout << center.x << " " << center.y << "\n";
	float button_width = 0.2f * size.x;
	float button_height = 0.1f * size.y;
	unsigned int button_text_size = 25;
	main_menu_button.setButtonSize({ button_width, button_height });
	main_menu_button.setOrigin({ button_width / 2, button_height / 2 });
	main_menu_button.setPosition({ size.x / 2.f, size.y * 0.6f });
	main_menu_button.setCharacterSize(button_text_size);

	linked_list_button.setButtonSize(main_menu_button.getSize());
	linked_list_button.setOrigin(main_menu_button.getOrigin());
	linked_list_button.setPosition({ size.x / 2.f, size.y * 0.8f });
	linked_list_button.setCharacterSize(button_text_size);
	//std::cout << main_menu_button.getPosition().x << " " << main_menu_button.getPosition().y << "\n";
	//std::cout << main_menu_button.getOrigin().x << " " << main_menu_button.getOrigin().y << "\n";
	//std::cout << button_width << " " << button_height << "\n";
}

void DSMenu::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	main_menu_button.update(window, view);
	linked_list_button.update(window, view);
}