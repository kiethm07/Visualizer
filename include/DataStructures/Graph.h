#pragma once
#include <Model/GraphModel/GraphState.h>
#include <Model/GraphModel/GraphOperation.h>
#include <Animation/Graph/GraphRecorder.h>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>

class Graph {
public:
	GraphState getState() const;
	void loadState(const GraphState& state);
	void applyOperation(const GraphOperation& operation, GraphRecorder& recorder);
	void rawInit(int node_cnt, const std::vector<int>& node_list, const std::vector<std::tuple<int, int, int>>& edges);
	void saveToFile(const std::string& filepath) const;
	void loadFromFile(const std::string& file_path);
private:
	int next_ui_id = 0;
	struct Node {
		int ui_id;
		std::map<int, int> neighbors; //to ui_id
		Node() {}
		Node(int id) :
			ui_id(id) {
		}
	};
	std::map<int, Node> nodes;
	std::map<std::pair<int, int>, int> edges;
	void clearWithoutRecorder();
	void clear(GraphRecorder& recorder);
	void insertNode(int key, GraphRecorder& recorder);
	void removeNode(int key, GraphRecorder& recorder);
	void addEdge(int from, int to, int weight, GraphRecorder& recorder);
	void removeEdge(int from, int to, GraphRecorder& recorder);
	void modifyEdge(int from, int to, int new_weight, GraphRecorder& recorder);
	void runDijkstra(int start, GraphRecorder& recorder) const;
	void runKruskal(GraphRecorder& recorder) const;
};