#include <Model/LinkedListModel/LinkedListRenderer.h>
#include <iostream>

LinkedListRenderer::LinkedListRenderer(const AssetManager& a_manager) :
	a_manager(a_manager)
{
}

static int string_to_int(const std::string& s) {
	if (s.empty()) return 0;
	bool sign = s[0] == '-';
	int res = 0;
	for (int i = sign; i < s.size(); i++) {
		res = res * 10 + s[i] - '0';
	}
	if (sign) res = -res;
	return res;
}

static std::string int_to_string(int n) {
	std::string res = "";
	bool sign = n < 0;
	if (sign) n = -n;
	while (n) {
		char c = n % 10 + '0';
		res += c;
		n /= 10;
	}
	if (res.empty()) res += '0';
	if (sign) res += '-';
	reverse(res.begin(), res.end());
	return res;
}

void LinkedListRenderer::loadState(const LinkedListAnimationState& animation_state) {
	node_list.clear();
	edge_list.clear();
	std::vector<LinkedListAnimationNode> nodes;
	std::vector<LinkedListAnimationEdge> edges;
	nodes = animation_state.getNodeList();
	edges = animation_state.getEdgeList();
	for (int i = 0; i < nodes.size(); i++){
		std::string label = int_to_string(nodes[i].value);
		ListNode node(a_manager.getFont("Roboto-Regular"), label, nodes[i].position, NODE_RADIUS, 20);
		//Default color is green, but since the animation state may have some node highlighted, we need to set the color according to the animation state
		node.setListNodeColor(nodes[i].fill_color);
		node_list.push_back(node);
	}
	for (int i = 0; i < edges.size(); i++) {
		Arrow edge(edges[i].from_position, edges[i].to_position, 5.f, 16.f, 12.f);
		edge.setColor(edges[i].fill_color);
		edge_list.push_back(edge);
	}
	//std::cout << "Loaded animation state with " << node_list.size() << " nodes and " << edge_list.size() << " edges\n";
}

void LinkedListRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
	for (int i = 0; i < node_list.size(); i++) {
		window.draw(node_list[i]);
	}
	for (int i = 0; i < edge_list.size(); i++) {
		window.draw(edge_list[i]);
	}
}