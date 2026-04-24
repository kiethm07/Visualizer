#pragma once
#include <Model/GraphModel/GraphState.h>

struct GraphAnimationNode {
	int value;
	int ui_id;
	int alpha;
	bool disable_physics = false;
	sf::Color fill_color;
};

struct GraphAnimationEdge {
	int from_ui_id;
	int to_ui_id;
	int alpha;
	bool disable_physics = false;
	sf::Color fill_color;
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
	void setEdgeList(const std::vector<GraphAnimationEdge>& edge_list) {
		this->edge_list = edge_list;
	}
	void setNodeList(const std::vector<GraphAnimationNode>& node_list) {
		this->node_list = node_list;
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
};