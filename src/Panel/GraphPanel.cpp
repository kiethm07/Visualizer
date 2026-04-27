#include <Panel/GraphPanel.h>

GraphPanel::GraphPanel(const AssetManager& a_manager) :
	a_manager(a_manager),
	BUTTON_FONT(a_manager.getFont("Roboto-Regular")),
	input_value(a_manager, "value", {}, {}, 20, 0),
	insert_node_button(a_manager, "Insert Node", {}, {}, 20),
	remove_node_button(a_manager, "Remove Node", {}, {}, 20),
	insert_edge_button(a_manager, "Insert Edge", {}, {}, 20),
	remove_edge_button(a_manager, "Remove Edge", {}, {}, 20),
	modify_edge_button(a_manager, "Modify Edge", {}, {}, 20),
	dijkstra_button(a_manager, "Run Dijkstra", {}, {}, 20),
	kruskal_button(a_manager, "Run Kruskal", {}, {}, 20),
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
	input_value.setMaxLength(69);
}

sf::Vector2f GraphPanel::getSize() {
	return background.getSize();
}

void GraphPanel::update(const sf::RenderWindow& window, const sf::View& view) {
	updateButtonState(window, view);
	updateWindowState(window, view);
}

void GraphPanel::updateButtonState(const sf::RenderWindow& window, const sf::View& view) {
	input_value.update(window, view);
	insert_node_button.update(window, view);
	remove_node_button.update(window, view);
	insert_edge_button.update(window, view);
	remove_edge_button.update(window, view);
	modify_edge_button.update(window, view);
	dijkstra_button.update(window, view);
	kruskal_button.update(window, view);
	reset_button.update(window, view);
	save_button.update(window, view);
	load_button.update(window, view);
	home_button.update(window, view);
	setting_button.update(window, view);
}

void GraphPanel::updateWindowState(const sf::RenderWindow& window, const sf::View& view) {
	sf::Vector2u size = window.getSize();

	sf::Vector2f background_size = { size.x * 0.15f, size.y * 0.85f };
	sf::Vector2f button_size = { background_size.x * 0.85f, background_size.y * 0.07f };

	background.setSize(background_size);

	const float center_x = background_size.x / 2.f;
	const float start_y = background_size.y * 0.08f;
	const float gap = background_size.y * 0.095f;

	auto place_button = [&](auto& btn, int index) {
		btn.setButtonSize(button_size);
		btn.setOrigin(button_size / 2.f);
		btn.setPosition({ center_x, start_y + gap * index });
		};

	place_button(input_value, 0);
	place_button(insert_node_button, 1);
	place_button(remove_node_button, 2);
	place_button(insert_edge_button, 3);
	place_button(remove_edge_button, 4);
	place_button(modify_edge_button, 5);
	place_button(dijkstra_button, 6);
	place_button(kruskal_button, 7);
	place_button(reset_button, 8);

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

std::optional<GraphOperation> GraphPanel::handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
	input_value.handleEvent(window, view, ev);

	auto parseInput = [](const std::string& input) -> std::vector<int> {
		std::vector<int> result;
		auto isDigit = [](char c) -> bool { return c >= '0' && c <= '9'; };
		auto toInt = [](char c) -> int { return c - '0'; };
		int currentNum = 0;
		int digitCount = 0;
		bool hasValue = false;
		for (char c : input) {
			if (isDigit(c)) {
				if (digitCount < 5) {
					currentNum = currentNum * 10 + toInt(c);
					digitCount++;
					hasValue = true;
				}
			}
			else {
				if (hasValue) {
					result.push_back(currentNum);
					currentNum = 0;
					digitCount = 0;
					hasValue = false;
					if (result.size() == 3) break;
				}
			}
		}
		if (hasValue) result.push_back(currentNum);
		return result;
	};

	if (insert_node_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 1) return std::nullopt;
		input_value.reset();
		return GraphOperation::insertNode(parsed[0]);
	}

	if (remove_node_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 1) return std::nullopt;
		input_value.reset();
		return GraphOperation::removeNode(parsed[0]);
	}

	if (insert_edge_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 3) return std::nullopt;
		input_value.reset();
		return GraphOperation::insertEdge(parsed[0], parsed[1], parsed[2]);
	}

	if (remove_edge_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 2) return std::nullopt;
		input_value.reset();
		return GraphOperation::removeEdge(parsed[0], parsed[1]);
	}

	if (modify_edge_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 3) return std::nullopt;
		input_value.reset();
		return GraphOperation::modifyEdge(parsed[0], parsed[1], parsed[2]);
	}

	if (dijkstra_button.handleEvent(window, view, ev)) {
		std::optional<std::string> value = input_value.getValue();
		if (!value.has_value()) return std::nullopt;
		std::vector<int> parsed = parseInput(*value);
		if (parsed.size() < 1) return std::nullopt;
		input_value.reset();
		return GraphOperation::dijkstra(parsed[0]);
	}

	if (kruskal_button.handleEvent(window, view, ev)) {
		return GraphOperation::kruskal();
	}

	if (reset_button.handleEvent(window, view, ev)) {
		return GraphOperation::reset();
	}

	if (home_button.handleEvent(window, view, ev)) {
		return GraphOperation::home();
	}

	if (setting_button.handleEvent(window, view, ev)) {
		return GraphOperation::setting();
	}

	if (save_button.handleEvent(window, view, ev)) {
		return GraphOperation::save(cr::utils::SimpleFileDialog::saveDialog());
	}

	if (load_button.handleEvent(window, view, ev)) {
		return GraphOperation::load(cr::utils::SimpleFileDialog::dialog());
	}

	return std::nullopt;
}