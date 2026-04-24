#include <Model/GraphModel/GraphRenderer.h>

GraphRenderer::GraphRenderer(const AssetManager& a_manager) :
	a_manager(a_manager) {
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

//void GraphRenderer::loadState(const GraphAnimationState& animation_state) {
//	node_list.clear();
//	edge_list.clear();
//	const std::vector<GraphAnimationNode>& nodes = animation_state.getNodeList();
//	const std::vector<GraphAnimationEdge>& edges = animation_state.getEdgeList();
//	for (int i = 0; i < nodes.size(); i++) {
//		//ListNode node(a_manager.getFont("Roboto-Regular"), int_to_string(nodes[i].value), nodes[i].position, NODE_RADIUS, 20);
//		//sf::Color node_color = nodes[i].fill_color;
//		//node_color.a = nodes[i].alpha;
//		//node.setListNodeColor(node_color);
//		//node.setOutlineColor(sf::Color(255, 255, 255, nodes[i].alpha));
//		//node.setCharacterColor(sf::Color(255, 255, 255, nodes[i].alpha));
//		//node_list.push_back(node);
//	}  
//	for (int i = 0; i < edges.size(); i++) {
//		//Arrow edge(edges[i].from_position, edges[i].to_position, 3.f, 14.f, 10.f);
//		//sf::Color edge_color = edges[i].fill_color;
//		//edge_color.a = edges[i].alpha;
//		//edge.setColor(edge_color);
//		//edge_list.push_back(edge);
//	}
//}

void GraphRenderer::loadState(const GraphAnimationState& animation_state) {
	current_state = animation_state;
}

//void GraphRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
//	for (int i = 0; i < node_list.size(); i++) {
//		window.draw(node_list[i]);
//	}
//	for (int i = 0; i < edge_list.size(); i++) {
//		window.draw(edge_list[i]);
//	}
//}

void GraphRenderer::draw(sf::RenderWindow& window, const sf::View& view) {
    // 1. Calculate delta time for physics
    float dt = physics_clock.restart().asSeconds();
    if (dt > 0.1f) dt = 0.1f; // Prevent physics explosion on lag spikes

    // 2. Update physics simulation
    physics.update(current_state, dt);

    const std::vector<GraphAnimationNode>& nodes = current_state.getNodeList();
    const std::vector<GraphAnimationEdge>& edges = current_state.getEdgeList();

    // 3. Draw Edges first (so they appear behind nodes)
    for (int i = 0; i < edges.size(); i++) {
        sf::Vector2f u_pos = physics.getPosition(edges[i].from_ui_id);
        sf::Vector2f v_pos = physics.getPosition(edges[i].to_ui_id);

        Arrow edge(u_pos, v_pos, 3.f, 14.f, NODE_RADIUS);
        sf::Color edge_color = edges[i].fill_color;
        edge_color.a = edges[i].alpha;
        edge.setColor(edge_color);

        window.draw(edge);
    }

    // 4. Draw Nodes
    for (int i = 0; i < nodes.size(); i++) {
        sf::Vector2f pos = physics.getPosition(nodes[i].ui_id);

        ListNode node(a_manager.getFont("Roboto-Regular"), int_to_string(nodes[i].value), pos, NODE_RADIUS, 20);
        sf::Color node_color = nodes[i].fill_color;
        node_color.a = nodes[i].alpha;

        node.setListNodeColor(node_color);
        node.setOutlineColor(sf::Color(255, 255, 255, nodes[i].alpha));
        node.setCharacterColor(sf::Color(255, 255, 255, nodes[i].alpha));

        window.draw(node);
    }
}