#include <Model/LinkedListModel/LinkedListRenderer.h>
#include <iostream>

LinkedListRenderer::LinkedListRenderer(const AssetManager& a_manager) :
	a_manager(a_manager)
{
}

static int string_to_int(const std::string& s) {
	int res = 0;
	for (int i = int(s.size()) - 1; i >= 0; i--) {
		res = res * 10 + s[i] - '0';
	}
	return res;
}

static std::string int_to_string(int n) {
	std::string res = "";
	while (n) {
		char c = n % 10 + '0';
		res += c;
		n /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

void LinkedListRenderer::loadState(const LinkedListState& list_state) {
	node_list.clear();
	edge_list.clear();
	int n = list_state.value.size();
	const std::vector<int>& value = list_state.value;
	const std::vector<int>& ui_id = list_state.ui_id;
	float x = X_MARGIN;
	float y = Y_MARGIN;
	for (int i = 0; i < n; i++){
		std::string label = int_to_string(value[i]);
		ListNode node(a_manager.getFont("Roboto-Regular"), label, sf::Vector2f{x, y}, NODE_RADIUS, 20);
		x += NODE_GAP;
		if (i > 0) {
			sf::Vector2f prev_pos = node_list.back().getPosition();
			sf::Vector2f cur_pos = node.getPosition();

			sf::Vector2f from = prev_pos + sf::Vector2f{ (float)NODE_RADIUS, 0.f};
			sf::Vector2f to = cur_pos - sf::Vector2f{ (float)NODE_RADIUS, 0.f};

			//std::cout << from.x << " " << from.y << " " << to.x << " " << to.y << " " << prev_pos.x << " " << prev_pos.y << "\n";

			Arrow edge(from, to, 5.f, 16.f, 12.f);
			edge.setColor(sf::Color::White);
			edge_list.push_back(edge);
		}
		node_list.push_back(node);
	}
}

void LinkedListRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
	for (int i = 0; i < node_list.size(); i++) {
		window.draw(node_list[i]);
	}
	for (int i = 0; i < edge_list.size(); i++) {
		window.draw(edge_list[i]);
	}
}