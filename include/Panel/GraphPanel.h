#pragma once
#include <Model/Button.h>
#include <Model/TextBox.h>
#include <Model/GraphModel/GraphOperation.h>
#include <External/SimpleFileDialog.h>
#include <optional>

class GraphPanel : public sf::Drawable {
public:
	GraphPanel(const sf::Font& BUTTON_FONT);

	sf::Vector2f getSize();
	void update(const sf::RenderWindow& window, const sf::View& view);
	std::optional<GraphOperation> handleEvent(const sf::RenderWindow& window, const sf::View& view, const sf::Event& ev);

private:
	const sf::Font& BUTTON_FONT;

	sf::RectangleShape background;

	TextBox input_value;

	Button insert_node_button;
	Button remove_node_button;
	Button insert_edge_button;
	Button remove_edge_button;
	Button modify_edge_button;
	Button dijkstra_button;
	Button kruskal_button;
	Button reset_button;

	Button save_button;
	Button load_button;
	Button home_button;
	Button setting_button;

	void updateButtonState(const sf::RenderWindow& window, const sf::View& view);
	void updateWindowState(const sf::RenderWindow& window, const sf::View& view);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		target.draw(background, states);
		target.draw(input_value, states);
		target.draw(insert_node_button, states);
		target.draw(remove_node_button, states);
		target.draw(insert_edge_button, states);
		target.draw(remove_edge_button, states);
		target.draw(modify_edge_button, states);
		target.draw(dijkstra_button, states);
		target.draw(kruskal_button, states);
		target.draw(reset_button, states);
		target.draw(home_button, states);
		target.draw(setting_button, states);
		target.draw(save_button, states);
		target.draw(load_button, states);
	}
};