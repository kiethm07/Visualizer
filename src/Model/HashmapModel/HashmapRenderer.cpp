#include <Model/HashmapModel/HashmapRenderer.h>
#include <iostream>

HashmapRenderer::HashmapRenderer(const AssetManager& asset_manager) :
	a_manager(asset_manager)
{
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

void HashmapRenderer::loadState(const HashmapAnimationState& animation_state) {
	node_list.clear();
	edge_list.clear();
	bucket_shape_list.clear();
	bucket_text_list.clear();
	std::vector<HashmapAnimationNode> nodes;
	std::vector<HashmapAnimationEdge> edges;
	std::vector<HashmapAnimationBucket> buckets;
	nodes = animation_state.getNodeList();
	edges = animation_state.getEdgeList();
	buckets = animation_state.getBucketList();

	for (int i = 0; i < buckets.size(); i++) {
		sf::RectangleShape shape(sf::Vector2f(100.f, 100.f));
		shape.setPosition(buckets[i].position);
		shape.setFillColor(buckets[i].fill_color);
		shape.setOutlineThickness(2.f);
		shape.setOutlineColor(sf::Color::White);
		bucket_shape_list.push_back(shape);

		sf::Text label(a_manager.getFont("Roboto-Regular"), int_to_string(i), 30);
		label.setFillColor(sf::Color::White);
		// Center the text roughly in the 100x100 bucket
		label.setPosition(buckets[i].position + sf::Vector2f(10.f, 10.f));
		bucket_text_list.push_back(label);
	}

	for (int i = 0; i < nodes.size(); i++) {
		std::string label = int_to_string(nodes[i].value);
		ListNode node(a_manager.getFont("Roboto-Regular"), label, nodes[i].position, 50, 20);
		sf::Color node_color = nodes[i].fill_color;
		node_color.a = nodes[i].alpha;
		node.setListNodeColor(node_color);
		node.setCharacterColor(sf::Color(255, 255, 255, nodes[i].alpha));
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

void HashmapRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
	for (int i = 0; i < bucket_shape_list.size(); i++) {
		window.draw(bucket_shape_list[i]);
		window.draw(bucket_text_list[i]);
	}
	for (int i = 0; i < node_list.size(); i++) {
		window.draw(node_list[i]);
	}
	for (int i = 0; i < edge_list.size(); i++) {
		window.draw(edge_list[i]);
	}
}