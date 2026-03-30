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
	input_value(BUTTON_FONT, "value", {}, {}, 20, 0),
	input_position(BUTTON_FONT, "position", {}, {}, 20, 0),
	insert_button(BUTTON_FONT, "insert", {}, {}, 20),
	remove_button(BUTTON_FONT, "remove", {}, {}, 20),
	update_button(BUTTON_FONT, "update", {}, {}, 20),
	reset_button(BUTTON_FONT, "reset", {}, {}, 20)
{
	background.setFillColor(sf::Color::White);
	background.setOrigin({ 0, 0 });
	background.setPosition({ 0, 0 });
	background.setSize({ 0, 0 });
}

sf::Vector2f LinkedListPanel::getSize() {
	return background.getSize();
}

void LinkedListPanel::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowState(window, view);
}

void LinkedListPanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	input_value.update(window, view);
	input_position.update(window, view);
	insert_button.update(window, view);
	remove_button.update(window, view);
	update_button.update(window, view);
	reset_button.update(window, view);
}

void LinkedListPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();

	sf::Vector2f background_size = { size.x * 0.20f, size.y * 0.85f };
	sf::Vector2f button_size = { background_size.x * 0.75f, background_size.y * 0.09f };

	background.setSize(background_size);

	const float center_x = background_size.x / 2.f;
	const float start_y = background_size.y * 0.15f;
	const float gap = background_size.y * 0.13f;

	auto place_button = [&](auto& btn, int index) {
		btn.setButtonSize(button_size);
		btn.setOrigin(button_size / 2.f);
		btn.setPosition({ center_x, start_y + gap * index });
		};

	place_button(input_value, 0);
	place_button(input_position, 1);
	place_button(insert_button, 2);
	place_button(remove_button, 3);
	place_button(update_button, 4);
	place_button(reset_button, 5);
}

std::optional<ListOperation> LinkedListPanel::handleEvent(
	const sf::RenderWindow& window,
	const sf::View& view,
	const sf::Event& ev
) {
	input_value.handleEvent(window, view, ev);
	input_position.handleEvent(window, view, ev);

	if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
		if (mb->button == sf::Mouse::Button::Left) {
			const sf::Vector2f mouse_pos = sf::Vector2f(mb->position);

			if (insert_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				std::optional<int> position = input_position.getValueAsInt();

				if (value.has_value() && position.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					input_position.setFocused(0);
					input_position.reset();
					return ListOperation::insertSingle(*position, *value);
				}

				return std::nullopt;
			}

			if (remove_button.contains(window, view, mouse_pos)) {
				std::optional<int> position = input_position.getValueAsInt();

				if (position.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					input_position.setFocused(0);
					input_position.reset();
					return ListOperation::remove(*position);
				}

				return std::nullopt;
			}

			if (update_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				std::optional<int> position = input_position.getValueAsInt();

				if (value.has_value() && position.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					input_position.setFocused(0);
					input_position.reset();
					return ListOperation::update(*position, *value);
				}

				return std::nullopt;
			}

			if (reset_button.contains(window, view, mouse_pos)) {
				return ListOperation::reset();
			}
		}
	}

	return std::nullopt;
}