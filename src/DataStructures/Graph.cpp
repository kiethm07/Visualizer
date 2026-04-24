#include <DataStructures/Graph.h>
#include <queue>
#include <algorithm>
#include <cmath>

GraphState Graph::getState() const {
	GraphState state;
	state.next_ui_id = next_ui_id;
	for (const auto& [key, node] : nodes) {
		state.nodes[key] = GraphState::Node(node.ui_id);
		state.nodes[key].neighbors = node.neighbors;
	}
	for (const auto& [p, w] : edges) {
		state.edges[p] = w;
	}
	std::cout << "GraphState: " << state.nodes.size() << " nodes, " << state.edges.size() << " edges\n";
	return state;
}

void Graph::loadState(const GraphState& state) {
	next_ui_id = state.next_ui_id;
	nodes.clear();
	edges.clear();
	for (const auto& [key, node] : state.nodes) {
		nodes[key] = Node(node.ui_id);
		nodes[key].neighbors = node.neighbors;
	}
	for (const auto& [p, w] : state.edges) {
		edges[p] = w;
	}
}

void Graph::applyOperation(const GraphOperation& operation, GraphRecorder& recorder) {
	if (operation.type == GraphOperationType::InsertNode) {
		insertNode(operation.value, recorder);
	}
	else if (operation.type == GraphOperationType::RemoveNode) {
		removeNode(operation.value, recorder);
	}
	else if (operation.type == GraphOperationType::InsertEdge) {
		addEdge(operation.from, operation.to, operation.weight, recorder);
	}
	else if (operation.type == GraphOperationType::RemoveEdge) {
		removeEdge(operation.from, operation.to, recorder);
	}
	else if (operation.type == GraphOperationType::ModifyEdge) {
		modifyEdge(operation.from, operation.to, operation.weight, recorder);
	}
	else if (operation.type == GraphOperationType::Dijkstra) {
		runDijkstra(operation.value, recorder);
	}
	else if (operation.type == GraphOperationType::Kruskal) {
		runKruskal(recorder);
	}
	else if (operation.type == GraphOperationType::Reset) {
		clear(recorder);
	}
}

void Graph::rawInit(const std::vector<int>& nodes, const std::vector<std::tuple<int, int, int>>& edges) {
	clearWithoutRecorder();
	std::set<std::pair<int, int>> added_edges;
	for (int value : nodes) {
		insertNode(value, GraphRecorder());
	}
	for (const auto& [u, v, w] : edges) {
		int x = u;
		int y = v;
		if (x > y) std::swap(x, y);
		if (added_edges.count({x,y})) continue;
		addEdge(x, y, w, GraphRecorder());
		added_edges.insert({ x, y });
	}
}

void Graph::clearWithoutRecorder() {
	nodes.clear();
	edges.clear();
	next_ui_id = 0;
}

void Graph::clear(GraphRecorder& recorder) {
	clearWithoutRecorder();
}

void Graph::insertNode(int key, GraphRecorder& recorder) {
	if (nodes.find(key) != nodes.end()) return;
	nodes[key] = Node(next_ui_id++);
}

void Graph::removeNode(int key, GraphRecorder& recorder) {
	if (nodes.find(key) == nodes.end()) return;

	for (const auto& [neighbor_id, weight] : nodes[key].neighbors) {
		int u = std::min(key, neighbor_id);
		int v = std::max(key, neighbor_id);
		edges.erase({ u, v });
		nodes[neighbor_id].neighbors.erase(key);
	}
	nodes.erase(key);
}

void Graph::addEdge(int from, int to, int weight, GraphRecorder& recorder) {
	int u = std::min(from, to);
	int v = std::max(from, to);
	if (edges.find({ u, v }) != edges.end()) return;
	edges[{u, v}] = weight;
	nodes[u].neighbors[v] = weight;
	nodes[v].neighbors[u] = weight;
}

void Graph::removeEdge(int from, int to, GraphRecorder& recorder) {
	int u = std::min(from, to);
	int v = std::max(from, to);
	edges.erase({ u, v });
	nodes[u].neighbors.erase(v);
	nodes[v].neighbors.erase(u);
}

void Graph::modifyEdge(int from, int to, int new_weight, GraphRecorder& recorder) {
	int u = std::min(from, to);
	int v = std::max(from, to);
	if (edges.find({ u, v }) != edges.end()) {
		edges[{u, v}] = new_weight;
		nodes[u].neighbors[v] = new_weight;
		nodes[v].neighbors[u] = new_weight;
	}
}

void Graph::runDijkstra(int start,  GraphRecorder& recorder) const {
	const int INF = 1e9;
	
	if (nodes.find(start) == nodes.end()) return;

	std::map<int, int> dist;
	std::map<int, int> parent;
	for (const auto& [id, node] : nodes) {
		dist[id] = INF;
	}
	dist[start] = 0;

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
	pq.push({ 0, start });

	while (!pq.empty()) {
		auto [d, u] = pq.top();
		pq.pop();

		if (d > dist[u]) continue;

		if (nodes.find(u) != nodes.end()) {
			for (const auto& [v, weight] : nodes.at(u).neighbors) {
				if (dist[u] + weight < dist[v]) {
					dist[v] = dist[u] + weight;
					parent[v] = u;
					pq.push({ dist[v], v });
				}
			}
		}
	}
}

void Graph::runKruskal(GraphRecorder& recorder) const {
	std::vector<std::pair<int, std::pair<int, int>>> edge_list;
	for (const auto& [p, w] : edges) {
		edge_list.push_back({ w, p });
	}
	std::sort(edge_list.begin(), edge_list.end());
	std::map<int, int> parent;
	for (const auto& [value, node] : nodes) {
		parent[value] = -1;
	}
	auto root = [&](auto&& self, int u) -> int {
		return parent[u] < 0 ? u : parent[u] = self(self, parent[u]);
	};
	auto join = [&](int u, int v) -> bool {
		u = root(root, u);
		v = root(root, v);
		if (u == v) return 0;
		if (parent[u] > parent[v]) std::swap(u, v);
		parent[u] += parent[v];
		parent[v] = u;
		return 1;
	};
	int MST = 0;
	for (const auto& [w, p] : edge_list) {
		int u = root(root, p.first);
		int v = root(root, p.second);
		if (join(u, v)) {
			MST += w;
		}
	}
}