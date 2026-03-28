#include <Panel/LinkedListPanel.h>
#include <iostream>
#include <chrono>
#include <random>

std::mt19937 rng(6969);

static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l,r)(rng);
}

LinkedListPanel::LinkedListPanel(const sf::Font& BUTTON_FONT) :
	BUTTON_FONT(BUTTON_FONT),
	reset_button(BUTTON_FONT, "reset", {}, {}, 20),
	insert_last_button(BUTTON_FONT, "insert last", {}, {}, 20),
	remove_last_button(BUTTON_FONT, "remove last", {}, {}, 20),
	insert_value(BUTTON_FONT, "insert val", {}, {}, 20)
{
	background.setFillColor(sf::Color::White);
	background.setOrigin({ 0,0 });
	background.setPosition({ 0,0 });
	background.setSize({ 0,0 });
}

sf::Vector2f LinkedListPanel::getSize() {
	return background.getSize();
}

void LinkedListPanel::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowState(window, view);
}

void LinkedListPanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	reset_button.update(window, view);
	insert_last_button.update(window, view);
	remove_last_button.update(window, view);
	insert_value.update(window, view);
}

void LinkedListPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();
	sf::Vector2f background_size = { size.x * 0.20f, size.y * 0.85f};
	sf::Vector2f button_size = { background_size.x * 0.75f, background_size.y * 0.100f };
	float button_gap = size.y * 0.075f;
	background.setSize(background_size);
	reset_button.setButtonSize(button_size);
	reset_button.setOrigin(button_size / 2.f);
	reset_button.setPosition({ background_size.x / 2.f, background_size.y * 0.2f });
	insert_last_button.setButtonSize(button_size);
	insert_last_button.setOrigin(button_size / 2.f);
	insert_last_button.setPosition({ background_size.x / 2.f, background_size.y * 0.4f });
	remove_last_button.setButtonSize(button_size);
	remove_last_button.setOrigin(button_size / 2.f);
	remove_last_button.setPosition({ background_size.x / 2.f, background_size.y * 0.6f });
	insert_value.setButtonSize(button_size);
	insert_value.setOrigin(button_size / 2.f);
	insert_value.setPosition({ background_size.x / 2.f, background_size.y * 0.8f });
}

std::optional<ListOperation> LinkedListPanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	insert_value.handleEvent(window, view, ev);
	if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
		if (mb->button == sf::Mouse::Button::Left) {
			if (insert_last_button.contains(window, view, sf::Vector2f(mb->position))) {
				std::cout << "insert clicked!\n";
				return ListOperation::insertSingle(0, rand(1,100));
			}

			if (remove_last_button.contains(window, view, sf::Vector2f(mb->position))) {
				std::cout << "remove clicked!\n";
				return ListOperation::erase(rand(0, 0));
			}

			if (reset_button.contains(window, view, sf::Vector2f(mb->position))) {
				std::cout << "reset clicked!\n";
				return ListOperation::reset();
			}

			if (insert_value.contains(window, view, sf::Vector2f(mb->position))) {
				std::cout << "insert clicked!\n";
				std::optional<int> value = insert_value.getValueAsInt();
				if (value.has_value()) {
					int x = *value;
					std::cout << x << "\n";
					return ListOperation::insertSingle(2, x);
				}

				return std::nullopt;
			}
		}
	}

	return std::nullopt;
}