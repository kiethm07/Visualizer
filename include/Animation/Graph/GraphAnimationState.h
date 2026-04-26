#pragma once
#include <Model/GraphModel/GraphState.h>

struct GraphAnimationNode {
	int value;
	int ui_id;
	int node_alpha;
	int value_alpha;
	bool disable_physics = false;
	sf::Color fill_color;
};

struct GraphAnimationEdge {
	int from_ui_id;
	int to_ui_id;
	int edge_alpha;
	int weight_alpha;
	int weight;
	bool in_spg = 0;
	bool flip_head = 0;
	bool disable_physics = false;
	sf::Color fill_color;
};

struct GraphAnimationPopup {
	int ui_id; //0 nothing, -1 Dijkstra, -2 MST
	int value = 0; // for MST only
	int value_alpha = 0;
	int popup_alpha = 0;
};

class GraphAnimationState {
public:
	GraphAnimationState() :
		edge_list(), node_list() {
	}
	const std::vector<GraphAnimationEdge>& getEdgeList() const {
		return edge_list;
	}
	const std::vector<GraphAnimationNode>& getNodeList() const {
		return node_list;
	}
	const GraphAnimationPopup getPopup() const {
		return pop_up;
	}
	void setEdgeList(const std::vector<GraphAnimationEdge>& edge_list) {
		this->edge_list = edge_list;
	}
	void setNodeList(const std::vector<GraphAnimationNode>& node_list) {
		this->node_list = node_list;
	}
	void setPopup(const GraphAnimationPopup& pop_up) {
		this->pop_up = pop_up;
	}
	void clear() {
		edge_list.clear();
		node_list.clear();
	}
	void modifyEdge(const GraphAnimationEdge& new_edge, int index) {
		if (index < 0 || index >= edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list[index] = new_edge;
	}
	void modifyNode(const GraphAnimationNode& new_node, int index) {
		if (index < 0 || index >= node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list[index] = new_node;
	}
	void insertNode(const GraphAnimationNode& new_node, int index) {
		if (index < 0 || index > node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list.insert(node_list.begin() + index, new_node);
	}
	void insertNode(const GraphAnimationNode& new_node) {
		node_list.push_back(new_node);
	}
	void insertEdge(const GraphAnimationEdge& new_edge, int index) {
		if (index < 0 || index > edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list.insert(edge_list.begin() + index, new_edge);
	}
	void insertEdge(const GraphAnimationEdge& new_edge) {
		edge_list.push_back(new_edge);
	}
private:
	std::vector<GraphAnimationEdge> edge_list;
	std::vector<GraphAnimationNode> node_list;
	GraphAnimationPopup pop_up;
};