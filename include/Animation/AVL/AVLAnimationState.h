#pragma once
#include <Model/AVLModel/AVLState.h>

struct AVLAnimationNode {
	int value;
	int ui_id;
	sf::Vector2f position;
	int alpha;
	sf::Color fill_color;
};

struct AVLAnimationEdge {
	int from_ui_id;
	int to_ui_id;
	sf::Vector2f from_position;
	sf::Vector2f to_position;
	int alpha;
	sf::Color fill_color;
};

class AVLAnimationState {
public:
	AVLAnimationState() :
		edge_list(), node_list() {
	}
	const std::vector<AVLAnimationEdge>& getEdgeList() const {
		return edge_list;
	}
	const std::vector<AVLAnimationNode>& getNodeList() const {
		return node_list;
	}
	void setEdgeList(const std::vector<AVLAnimationEdge>& edge_list) {
		this->edge_list = edge_list;
	}
	void setNodeList(const std::vector<AVLAnimationNode>& node_list) {
		this->node_list = node_list;
	}
	void clear() {
		edge_list.clear();
		node_list.clear();
	}
	void modifyEdge(const AVLAnimationEdge& new_edge, int index) {
		if (index < 0 || index >= edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list[index] = new_edge;
	}
	void modifyNode(const AVLAnimationNode& new_node, int index) {
		if (index < 0 || index >= node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list[index] = new_node;
	}
	void insertNode(const AVLAnimationNode& new_node, int index) {
		if (index < 0 || index > node_list.size()) {
			std::cout << "Node index " << index << " out of range" << std::endl;
			return;
		}
		node_list.insert(node_list.begin() + index, new_node);
	}
	void insertNode(const AVLAnimationNode& new_node) {
		node_list.push_back(new_node);
	}
	void insertEdge(const AVLAnimationEdge& new_edge, int index) {
		if (index < 0 || index > edge_list.size()) {
			std::cout << "Edge index " << index << " out of range" << std::endl;
			return;
		}
		edge_list.insert(edge_list.begin() + index, new_edge);
	}
	void insertEdge(const AVLAnimationEdge& new_edge) {
		edge_list.push_back(new_edge);
	}
private:
	std::vector<AVLAnimationEdge> edge_list;
	std::vector<AVLAnimationNode> node_list;
};