#pragma once
#include <SFML/Graphics.hpp>
#include <AssetManager/AssetManager.h>
#include <Panel/PseudoCodePanel.h>
#include <optional>

template<
	typename OperationType,
	typename Dictionary
>
class PseudoCodeController : public sf::Drawable {
public:
	PseudoCodeController(const AssetManager& asset_manager, const std::string& font_name)
		: panel(asset_manager, font_name), current_operation_type(std::nullopt) {
	}

	void setPosition(const sf::Vector2f& pos) {
		panel.setPosition(pos);
	}

	void update(const sf::RenderWindow& window, const sf::View& view) {
		panel.update(window, view);
	}

	void handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev) {
		panel.handleEvent(window, view, ev);
	}

	void sync(std::optional<OperationType> op_type, int highlighted_line) {
		if (!op_type.has_value()) {
			if (current_operation_type.has_value()) {
				current_operation_type = std::nullopt;
				panel.setCode({});
				panel.setTitle("No Operation");
				panel.setHighlight(-1);
			}
			return;
		}

		if (!current_operation_type.has_value() || current_operation_type.value() != op_type.value()) {
			current_operation_type = op_type.value();
			if (dictionary.titles.find(op_type.value()) != dictionary.titles.end()) {
				panel.setTitle(dictionary.titles[op_type.value()]);
			}
			else {
				panel.setTitle("Unknown Operation");
			}
			if (dictionary.pseudocode.find(op_type.value()) != dictionary.pseudocode.end()) {
				panel.setCode(dictionary.pseudocode[op_type.value()]);
			}
			else {
				panel.setCode({"Bug!"});
			}
		}

		panel.setHighlight(highlighted_line);
	}

private:
	PseudoCodePanel panel;
	Dictionary dictionary;
	std::optional<OperationType> current_operation_type;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(panel, states);
	}
};