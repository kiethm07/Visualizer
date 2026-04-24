#include <Panel/GraphPanel.h>

GraphPanel::GraphPanel(const sf::Font& BUTTON_FONT) :
	BUTTON_FONT(BUTTON_FONT),
	input_value(BUTTON_FONT, "value", {}, {}, 20, 0),
	insert_node_button(BUTTON_FONT, "InsertNode", {}, {}, 20),
	remove_node_button(BUTTON_FONT, "RemoveNode", {}, {}, 20),
	insert_edge_button(BUTTON_FONT, "InsertEdge", {}, {}, 20),
	remove_edge_button(BUTTON_FONT, "RemoveEdge", {}, {}, 20),
	modify_edge_button(BUTTON_FONT, "ModifyEdge", {}, {}, 20),
	dijkstra_button(BUTTON_FONT, "RunDijkstra", {}, {}, 20),
	kruskal_button(BUTTON_FONT, "RunKruskal", {}, {}, 20),
	reset_button(BUTTON_FONT, "Reset", {}, {}, 20)
{
	background.setFillColor(sf::Color::White);
	background.setOrigin({ 0, 0 });
	background.setPosition({ 0, 0 });
	background.setSize({ 0, 0 });
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
}

std::optional<GraphOperation> GraphPanel::handleEvent(
	const sf::RenderWindow& window,
	const sf::View& view,
	const sf::Event& ev
) {
	input_value.handleEvent(window, view, ev);

	if (const auto* mb = ev.getIf<sf::Event::MouseButtonReleased>()) {
		if (mb->button == sf::Mouse::Button::Left) {
			const sf::Vector2f mouse_pos = sf::Vector2f(mb->position);

			if (insert_node_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					return GraphOperation::insertNode(*value);
				}
				return std::nullopt;
			}

			if (remove_node_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					return GraphOperation::removeNode(*value);
				}
				return std::nullopt;
			}

			if (insert_edge_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					//return GraphOperation::insertEdge(*value, *value, *value);
				}
				return std::nullopt;
			}

			if (remove_edge_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					//return GraphOperation::removeEdge(*value, *value);
				}
				return std::nullopt;
			}

			if (modify_edge_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					return GraphOperation::modifyEdge(*value, *value, *value);
				}
				return std::nullopt;
			}

			if (dijkstra_button.contains(window, view, mouse_pos)) {
				std::optional<int> value = input_value.getValueAsInt();
				if (value.has_value()) {
					input_value.setFocused(0);
					input_value.reset();
					return GraphOperation::dijkstra(*value);
				}
				return std::nullopt;
			}

			if (kruskal_button.contains(window, view, mouse_pos)) {
				return GraphOperation::kruskal();
			}

			if (reset_button.contains(window, view, mouse_pos)) {
				return GraphOperation::reset();
			}
		}
	}

	return std::nullopt;
}