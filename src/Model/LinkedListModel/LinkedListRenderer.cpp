#include <Model/LinkedListModel/LinkedListRenderer.h>

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
	int n = list_state.value.size();
	const std::vector<int>& value = list_state.value;
	const std::vector<int>& ui_id = list_state.ui_id;
	int x = X_MARGIN;
	int y = Y_MARGIN;
	for (int i = 0; i < n; i++){
		std::string label = int_to_string(value[i]);
		ListNode node(a_manager.getFont("Roboto-Regular"), label, sf::Vector2f{float(x), float(y)}, node_size, 20);
		x += node_gap;
		vec_list.push_back(node);
	}
}

void LinkedListRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
	for (int i = 0; i < vec_list.size(); i++) {
		window.draw(vec_list[i]);
	}
}