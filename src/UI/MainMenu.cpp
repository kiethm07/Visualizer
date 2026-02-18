#include <UI/MainMenu.h>
#include <iostream>

MainMenu::MainMenu(const sf::Font& font) :
	menu_font(font),
	start_button(font, "Start", {}, {}, 20),
	setting_button(font, "Setting", {}, {}, 20) {}

void MainMenu::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowSize(window, view);
}

std::optional<MenuState> MainMenu::handleEvent(const sf::RenderWindow & window, const sf::View& view, const sf::Event& ev) {
	if (const auto* resized = ev.getIf<sf::Event::Resized>()) {
		updateWindowSize(window, view);
	}
	if (const auto* mouse_pressed = ev.getIf<sf::Event::MouseButtonPressed>()) {
		if (mouse_pressed->button == sf::Mouse::Button::Left) {
			if (start_button.mousePressed(window, view, ev)) {
				return MenuState::DSMenu;
			}
			if (setting_button.mousePressed(window, view, ev)) {
				return MenuState::Setting;
			}
		}
	}
	return std::nullopt;
}

void MainMenu::updateWindowSize(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();
	sf::Vector2f center = { size.x / 2.f, size.y / 2.f };
	//std::cout << center.x << " " << center.y << "\n";
	float button_width = 0.2f * size.x;
	float button_height = 0.1f * size.y;
	unsigned int button_text_size = 25;
	start_button.setButtonSize({ button_width, button_height });
	start_button.setOrigin({ button_width / 2, button_height / 2 });
	start_button.setPosition({ size.x / 2.f, size.y * 0.6f });
	start_button.setCharacterSize(button_text_size);

	setting_button.setButtonSize(start_button.getSize());
	setting_button.setOrigin(start_button.getOrigin());
	setting_button.setPosition({ size.x / 2.f, size.y * 0.8f });
	setting_button.setCharacterSize(button_text_size);
	//std::cout << start_button.getPosition().x << " " << start_button.getPosition().y << "\n";
	//std::cout << start_button.getOrigin().x << " " << start_button.getOrigin().y << "\n";
	//std::cout << button_width << " " << button_height << "\n";
}

void MainMenu::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	start_button.update(window, view);
	setting_button.update(window, view);
}