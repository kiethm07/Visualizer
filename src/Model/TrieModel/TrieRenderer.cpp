#include <Model/TrieModel/TrieRenderer.h>
#include <iostream>

TrieRenderer::TrieRenderer(const AssetManager& a_manager) :
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
void TrieRenderer::loadState(const TrieAnimationState& animation_state) {
	node_list.clear();
	edge_list.clear();
	std::vector<TrieAnimationNode> nodes;
	std::vector<TrieAnimationEdge> edges;
	nodes = animation_state.getNodeList();
	edges = animation_state.getEdgeList();
	for (int i = 0; i < nodes.size(); i++) {
		std::string label = nodes[i].value;
		ListNode node(a_manager.getFont("Roboto-Regular"), label, nodes[i].position, NODE_RADIUS, 20);
		sf::Color node_color = nodes[i].fill_color;
		node_color.a = nodes[i].alpha;
		node.setListNodeColor(node_color);
		node.setCharacterColor(sf::Color(255, 255, 255, nodes[i].alpha));
		node.setOutlineColor(sf::Color(255, 255, 255, nodes[i].alpha));
		node_list.push_back(node);
	}
	for (int i = 0; i < edges.size(); i++) {
		Arrow edge(edges[i].from_position, edges[i].to_position, 5.f, 16.f, 12.f);
		sf::Color edge_color = edges[i].fill_color;
		edge_color.a = edges[i].alpha;
		edge.setColor(edge_color);
		edge_list.push_back(edge);
	}
}
void TrieRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
	for (int i = 0; i < node_list.size(); i++) {
		window.draw(node_list[i]);
	}
	for (int i = 0; i < edge_list.size(); i++) {
		window.draw(edge_list[i]);
	}
}