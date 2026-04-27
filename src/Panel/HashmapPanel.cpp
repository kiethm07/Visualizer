#include <Panel/HashmapPanel.h>
#include <iostream>
#include <chrono>
#include <random>

static std::mt19937 rng(6969);

static int rand(int l, int r) {
	if (l > r) std::swap(l, r);
	return std::uniform_int_distribution<int>(l, r)(rng);
}

HashmapPanel::HashmapPanel(const AssetManager& a_manager) :
	a_manager(a_manager),
	BUTTON_FONT(a_manager.getFont("Roboto-Regular")),
	input_value(a_manager, "value", {}, {}, 20, 0),
	insert_button(a_manager, "insert", {}, {}, 20),
	remove_button(a_manager, "Remove", {}, {}, 20),
	search_button(a_manager, "Search", {}, {}, 20),
	reset_button(a_manager, "Reset", {}, {}, 20),

	home_button(a_manager, "HomeIcon", {}),
	setting_button(a_manager, "SettingIcon", {}),
	save_button(a_manager, "SaveFileIcon", {}),
	load_button(a_manager, "LoadFileIcon", {})
{
	background.setFillColor(sf::Color(240, 206, 155, 200));
	background.setOrigin({ 0, 0 });
	background.setPosition({ 0, 0 });
	background.setSize({ 0, 0 });
}

sf::Vector2f HashmapPanel::getSize() {
	return background.getSize();
}

void HashmapPanel::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowState(window, view);
}

void HashmapPanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	input_value.update(window, view);
	insert_button.update(window, view);
	remove_button.update(window, view);
	search_button.update(window, view);
	reset_button.update(window, view);
	save_button.update(window, view);
	load_button.update(window, view);
	home_button.update(window, view);
	setting_button.update(window, view);
}

void HashmapPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
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
	place_button(insert_button, 1);
	place_button(remove_button, 2);
	place_button(search_button, 3);
	place_button(reset_button, 4);

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

std::optional<HashmapOperation> HashmapPanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	input_value.handleEvent(window, view, ev);

	if (insert_button.handleEvent(window, view, ev)) {
		std::optional<int> value = input_value.getValueAsInt();
		if (value.has_value()) {
			input_value.setFocused(0);
			input_value.reset();
			return HashmapOperation::insert(*value, *value);
		}
		return std::nullopt;
	}

	if (remove_button.handleEvent(window, view, ev)) {
		std::optional<int> value = input_value.getValueAsInt();
		if (value.has_value()) {
			input_value.setFocused(0);
			input_value.reset();
			return HashmapOperation::remove(*value);
		}
		return std::nullopt;
	}

	if (search_button.handleEvent(window, view, ev)) {
		std::optional<int> value = input_value.getValueAsInt();
		if (value.has_value()) {
			input_value.setFocused(0);
			input_value.reset();
			std::cout << (*value) << "\n";
			return HashmapOperation::search(*value);
		}
		return std::nullopt;
	}

	if (reset_button.handleEvent(window, view, ev)) {
		return HashmapOperation::reset();
	}

	if (home_button.handleEvent(window, view, ev)) {
		return HashmapOperation::home();
	}

	if (setting_button.handleEvent(window, view, ev)) {
		return HashmapOperation::setting();
	}

	if (save_button.handleEvent(window, view, ev)) {
		return HashmapOperation::save(cr::utils::SimpleFileDialog::saveDialog());
	}

	if (load_button.handleEvent(window, view, ev)) {
		return HashmapOperation::load(cr::utils::SimpleFileDialog::dialog());
	}

	return std::nullopt;
}