#pragma once
#include <Model/HashmapModel/HashmapState.h>

struct HashmapAnimationNode {
	int value;
	int ui_id;
	sf::Vector2f position;
	int alpha;
	sf::Color fill_color;
};

struct HashmapAnimationEdge {
	int from_ui_id;
	int to_ui_id;
	sf::Vector2f from_position;
	sf::Vector2f to_position;
	int alpha;
	sf::Color fill_color;
};

class HashmapAnimationState {
public:
	HashmapAnimationState() :
		edge_list(), node_list() {
	}
	const std::vector<HashmapAnimationEdge>& getEdgeList() const {
		return edge_list;
	}
	const std::vector<HashmapAnimationNode>& getNodeList() const {
		return node_list;
	}
	void setEdgeList(const std::vector<HashmapAnimationEdge>& edge_list) {
		this->edge_list = edge_list;
	}
	void setNodeList(const std::vector<HashmapAnimationNode>& node_list) {
		this->node_list = node_list;
	}
	void clear() {
		edge_list.clear();
		node_list.clear();
	}
	void modifyEdge(const HashmapAnimationEdge& new_edge, int index) {
		if (index < 0 || index >= edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list[index] = new_edge;
	}
	void modifyNode(const HashmapAnimationNode& new_node, int index) {
		if (index < 0 || index >= node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list[index] = new_node;
	}
	void insertNode(const HashmapAnimationNode& new_node, int index) {
		if (index < 0 || index > node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list.insert(node_list.begin() + index, new_node);
	}
	void insertNode(const HashmapAnimationNode& new_node) {
		node_list.push_back(new_node);
	}
	void insertEdge(const HashmapAnimationEdge& new_edge, int index) {
		if (index < 0 || index > edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list.insert(edge_list.begin() + index, new_edge);
	}
	void insertEdge(const HashmapAnimationEdge& new_edge) {
		edge_list.push_back(new_edge);
	}
private:
	std::vector<HashmapAnimationEdge> edge_list;
	std::vector<HashmapAnimationNode> node_list;
};