#include <Panel/LinkedListPanel.h>
#include <iostream>
#include <chrono>
#include <random>

static std::mt19937 rng(6969);

static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l,r)(rng);
}

LinkedListPanel::LinkedListPanel(const AssetManager& a_manager) :
	a_manager(a_manager),
	BUTTON_FONT(a_manager.getFont("Roboto-Regular")),
	input_value(a_manager, "value", {}, {}, 20, 0),
	input_position(a_manager, "position", {}, {}, 20, 0),
	insert_button(a_manager, "Insert", {}, {}, 20),
	remove_button(a_manager, "Remove", {}, {}, 20),
	update_button(a_manager, "Update", {}, {}, 20),
	search_button(a_manager, "Search", {}, {}, 20),
	reset_button(a_manager, "Reset", {}, {}, 20),

	home_button(a_manager, "Home", {}, {}, 20),
	setting_button(a_manager, "Set", {}, {}, 20),
	save_button(a_manager, "Save", {}, {}, 20),
	load_button(a_manager, "Load", {}, {}, 20)
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
	search_button.update(window, view);
	reset_button.update(window, view);
	save_button.update(window, view);
	load_button.update(window, view);
	home_button.update(window, view);
	setting_button.update(window, view);
}

void LinkedListPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();

	sf::Vector2f background_size = { size.x * 0.15f, size.y * 0.85f };
	sf::Vector2f button_size = { background_size.x * 0.75f, background_size.y * 0.09f };

	background.setSize(background_size);

	const float center_x = background_size.x / 2.f;
	const float start_y = background_size.y * 0.1f;
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
	place_button(search_button, 5);
	place_button(reset_button, 6);

	auto place_system_button = [&](auto& btn, int index) {
		float side = background_size.x * 0.25f;
		sf::Vector2f sq_size = { side, side };

		btn.setButtonSize(sq_size);
		btn.setOrigin(sq_size / 2.f);

		float margin = 10.f;
		float pos_x = margin + sq_size.x / 2.f + (sq_size.x + 10.f) * index;
		float pos_y = size.y - margin - sq_size.y / 2.f;

		btn.setPosition({ pos_x, pos_y });
		};

	place_system_button(home_button, 0);
	place_system_button(setting_button, 1);
	place_system_button(save_button, 2);
	place_system_button(load_button, 3);
}

std::optional<ListOperation> LinkedListPanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	input_value.handleEvent(window, view, ev);
	input_position.handleEvent(window, view, ev);

	if (insert_button.handleEvent(window, view, ev)) {
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

	if (remove_button.handleEvent(window, view, ev)) {
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

	if (update_button.handleEvent(window, view, ev)) {
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

	if (search_button.handleEvent(window, view, ev)) {
		std::optional<int> value = input_value.getValueAsInt();
		if (value.has_value()) {
			input_value.setFocused(0);
			input_value.reset();
			input_position.setFocused(0);
			input_position.reset();
			return ListOperation::search(*value);
		}
		return std::nullopt;
	}

	if (reset_button.handleEvent(window, view, ev)) {
		return ListOperation::reset();
	}

	if (home_button.handleEvent(window, view, ev)) {
		return ListOperation::home();
	}

	if (setting_button.handleEvent(window, view, ev)) {
		return ListOperation::setting();
	}

	if (save_button.handleEvent(window, view, ev)) {
		return ListOperation::save(cr::utils::SimpleFileDialog::saveDialog());
	}

	if (load_button.handleEvent(window, view, ev)) {
		return ListOperation::load(cr::utils::SimpleFileDialog::dialog());
	}

	return std::nullopt;
}