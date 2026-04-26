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
	//std::cout << "GraphState: " << state.nodes.size() << " nodes, " << state.edges.size() << " edges\n";
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
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	if (operation.type == GraphOperationType::InsertNode) {
		insertNode(operation.value, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(3);
	}
	else if (operation.type == GraphOperationType::RemoveNode) {
		removeNode(operation.value, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(5);
	}
	else if (operation.type == GraphOperationType::InsertEdge) {
		addEdge(operation.from, operation.to, operation.weight, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(3);
	}
	else if (operation.type == GraphOperationType::RemoveEdge) {
		removeEdge(operation.from, operation.to, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(3);
	}
	else if (operation.type == GraphOperationType::ModifyEdge) {
		modifyEdge(operation.from, operation.to, operation.weight, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(2);
	}
	else if (operation.type == GraphOperationType::Dijkstra) {
		runDijkstra(operation.value, recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(13);
	}
	else if (operation.type == GraphOperationType::Kruskal) {
		runKruskal(recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(5);
	}
	else if (operation.type == GraphOperationType::Reset) {
		clear(recorder);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(1);
	}
	if (operation.type == GraphOperationType::Save) {
		saveToFile(operation.file_path);
		return;
	}
	if (operation.type == GraphOperationType::Load) {
		loadFromFile(operation.file_path);
		return;
	}
}

void Graph::saveToFile(const std::string& filepath) const {
	if (filepath.empty()) return;

	GraphState state = getState();

	std::ofstream fout(filepath);
	if (!fout.is_open()) {
		std::cerr << "Cannot open file: " << filepath << "\n";
		return;
	}

	fout << state.next_ui_id << " " << state.nodes.size() << " " << state.edges.size() << "\n";

	for (const auto& [key, node] : state.nodes) {
		fout << key << " "
			<< node.ui_id << " "
			<< node.neighbors.size() << "\n";
		for (const auto& [neigh_id, val] : node.neighbors) {
			fout << neigh_id << " " << val << " ";
		}
		fout << "\n";
	}

	for (const auto& [p, w] : state.edges) {
		fout << p.first << " " << p.second << " " << w << "\n";
	}

	fout.close();
	std::cout << "Success: " << filepath << "\n";
}

void Graph::loadFromFile(const std::string& filepath) {
	GraphState state;
	if (filepath.empty()) return;

	std::ifstream fin(filepath);
	if (!fin.is_open()) {
		std::cerr << "Cannot open file: " << filepath << "\n";
		return;
	}

	int node_count = 0;
	int edge_count = 0;
	if (fin >> state.next_ui_id >> node_count >> edge_count) {
		for (int i = 0; i < node_count; ++i) {
			int key, ui_id, sz;
			fin >> key >> ui_id >> sz;
			state.nodes[key] = GraphState::Node(ui_id);
			for (int j = 0; j < sz; ++j) {
				int neigh_id, val;
				fin >> neigh_id >> val;
				state.nodes[key].neighbors[neigh_id] = val;
			}
		}

		for (int i = 0; i < edge_count; ++i) {
			int u, v, w;
			fin >> u >> v >> w;
			state.edges[{u, v}] = w;
		}
	}

	fin.close();
	std::cout << "Success: " << filepath << "\n";
	loadState(state);
}

void Graph::rawInit(int node_cnt, const std::vector<std::tuple<int, int, int>>& edges) {
	clearWithoutRecorder();
	std::set<std::pair<int, int>> added_edges;
	//for (int value : nodes) {
	//	insertNode(value, GraphRecorder());
	//}
	GraphRecorder dummy;
	for (int i = 0; i < node_cnt; i++) {
		insertNode(i, dummy);
	}
	for (const auto& [u, v, w] : edges) {
		int x = u;
		int y = v;
		if (x > y) std::swap(x, y);
		if (added_edges.count({x,y})) continue;
		addEdge(x, y, w, dummy);
		added_edges.insert({ x, y });
	}
}

void Graph::clearWithoutRecorder() {
	nodes.clear();
	edges.clear();
	next_ui_id = 0;
}

void Graph::clear(GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	for (const auto& [value, u] : nodes) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, nodes[value].ui_id));
		for (const auto& [v, weight] : nodes.at(value).neighbors) {
			int x = std::min(value, v);
			int y = std::max(value, v);
			if (edges.find({ x,y }) != edges.end()) {
				recorder.addNewPhase();
				recorder.setHighlightedLine(0);
				recorder.addCommand(Command(Target::Edge, Type::FadeOut, nodes[x].ui_id, nodes[y].ui_id));
				edges.erase({ x,y });
			}
		}
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Node, Type::FadeOut, nodes[value].ui_id));
	}
	nodes.clear();
	edges.clear();
}

void Graph::insertNode(int key, GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	recorder.addNewPhase();
	recorder.addCommand(Command::createWaitCommand());
	recorder.setHighlightedLine(0);
	if (nodes.find(key) != nodes.end()) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, nodes[key].ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command::createWaitCommand());
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, nodes[key].ui_id));
		return;
	}
	nodes[key] = Node(next_ui_id++);
	recorder.addNewPhase();
	recorder.setHighlightedLine(2);
	recorder.addCommand(Command::createSpawnNodeCommand(nodes[key].ui_id, -1, key, { 0, 0 }));
	recorder.addCommand(Command(Target::Node, Type::FadeIn, nodes[key].ui_id));
}

void Graph::removeNode(int key, GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command::createWaitCommand());
	if (nodes.find(key) == nodes.end()) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command::createWaitCommand());
		return;
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(2);
	recorder.addCommand(Command(Target::Node, Type::HighlightOn, nodes[key].ui_id));
	for (const auto& [neighbor_id, weight] : nodes[key].neighbors) {
		int u = std::min(key, neighbor_id);
		int v = std::max(key, neighbor_id);
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Edge, Type::FadeOut, nodes[u].ui_id, nodes[v].ui_id));
		edges.erase({ u, v });
		nodes[neighbor_id].neighbors.erase(key);
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(4);
	recorder.addCommand(Command(Target::Node, Type::FadeOut, nodes[key].ui_id));
	nodes.erase(key);
}

void Graph::addEdge(int from, int to, int weight, GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	int u = std::min(from, to);
	int v = std::max(from, to);
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command::createWaitCommand());
	if (edges.find({ u, v }) != edges.end()) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Edge, Type::HighlightOn, nodes[u].ui_id, nodes[v].ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command::createWaitCommand());
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Edge, Type::HighlightOff, nodes[u].ui_id, nodes[v].ui_id));
		return;
	}
	edges[{u, v}] = weight;
	nodes[u].neighbors[v] = weight;
	nodes[v].neighbors[u] = weight;
	recorder.addNewPhase();
	recorder.setHighlightedLine(2);
	recorder.addCommand(Command::createSpawnEdgeCommand(nodes[u].ui_id, nodes[v].ui_id, weight));
	recorder.addCommand(Command(Target::Edge, Type::FadeIn, nodes[u].ui_id, nodes[v].ui_id));
}

void Graph::removeEdge(int from, int to, GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	int u = std::min(from, to);
	int v = std::max(from, to);
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command::createWaitCommand());
	if (edges.find({ u,v }) == edges.end()) {
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command::createWaitCommand());
		return;
	}
	edges.erase({ u, v });
	recorder.addNewPhase();
	recorder.setHighlightedLine(2);
	recorder.addCommand(Command(Target::Edge, Type::FadeOut, nodes[u].ui_id, nodes[v].ui_id));
	nodes[u].neighbors.erase(v);
	nodes[v].neighbors.erase(u);
}

void Graph::modifyEdge(int from, int to, int new_weight, GraphRecorder& recorder) {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	int u = std::min(from, to);
	int v = std::max(from, to);
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command::createWaitCommand());
	if (edges.find({ u, v }) != edges.end()) {
		edges[{u, v}] = new_weight;
		nodes[u].neighbors[v] = new_weight;
		nodes[v].neighbors[u] = new_weight;
		recorder.addNewPhase();
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command(Target::Edge, Type::HighlightOn, nodes[u].ui_id, nodes[v].ui_id));
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command::createChangeValueCommand(nodes[u].ui_id, nodes[v].ui_id, new_weight));
		recorder.addNewPhase();
		recorder.setHighlightedLine(1);
		recorder.addCommand(Command(Target::Edge, Type::HighlightOff, nodes[u].ui_id, nodes[v].ui_id));
	}
}

void Graph::runDijkstra(int start,  GraphRecorder& recorder) const {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	const int INF = 1e9;
	
	if (nodes.find(start) == nodes.end()) return;

	std::map<int, int> dist;
	std::map<int, std::vector<int>> parent;
	for (const auto& [id, node] : nodes) {
		dist[id] = INF;
	}
	dist[start] = 0;
	recorder.addNewPhase();
	for (const auto& [val, u] : nodes) {
		recorder.setHighlightedLine(0);
		recorder.addCommand(Command::createChangeValueCommand(u.ui_id, INF));
	}
	recorder.addNewPhase();
	recorder.setHighlightedLine(0);
	recorder.addCommand(Command::createChangeValueCommand(nodes.at(start).ui_id, 0));

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
	pq.push({ 0, start });
	recorder.addNewPhase();
	recorder.setHighlightedLine(1);
	recorder.addCommand(Command::createWaitCommand());

	while (!pq.empty()) {
		auto [d, u] = pq.top();
		pq.pop();
		recorder.addNewPhase();
		recorder.setHighlightedLine(2);
		recorder.addCommand(Command::createWaitCommand());
		if (d > dist[u]) continue;
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Node, Type::HighlightOn, nodes.at(u).ui_id));

		if (nodes.find(u) != nodes.end()) {
			for (const auto& [v, weight] : nodes.at(u).neighbors) {
				recorder.addNewPhase();
				recorder.setHighlightedLine(4);
				recorder.addCommand(Command::createWaitCommand());
				if (dist[u] > dist[v]) continue;
				int x = std::min(u, v);
				int y = std::max(u, v);
				x = nodes.at(x).ui_id;
				y = nodes.at(y).ui_id;
				recorder.addNewPhase();
				recorder.setHighlightedLine(5);
				recorder.addCommand(Command(Target::Edge, Type::HighlightOn, x, y));
				if (dist[u] + weight < dist[v]) {
					dist[v] = dist[u] + weight;
					recorder.addNewPhase();
					recorder.setHighlightedLine(6);
					recorder.addCommand(Command::createChangeValueCommand(nodes.at(v).ui_id, dist[v]));
					recorder.addNewPhase();
					for (int i = 0; i < parent[v].size(); i++) {
						int x = std::min(v, parent[v][i]);
						int y = std::max(v, parent[v][i]);
						x = nodes.at(x).ui_id;
						y = nodes.at(y).ui_id;
						recorder.setHighlightedLine(7);
						recorder.addCommand(Command(Target::Edge, Type::FadeOut, x, y));
					}
					parent[v].clear();
					recorder.addNewPhase();
					recorder.setHighlightedLine(8);
					recorder.addCommand(Command::createWaitCommand());
					pq.push({ dist[v], v });
				}
				recorder.addNewPhase();
				recorder.setHighlightedLine(9);
				recorder.addCommand(Command::createWaitCommand());
				if (dist[u] + weight == dist[v]) {
					Command com = Command(Target::Edge, Type::InSPG, x, y);
					com.flip_head = x == u ? 0 : 1;
					com.in_spg = 1;
					recorder.addNewPhase();
					recorder.setHighlightedLine(10);
					recorder.addCommand(com);
					parent[v].push_back(u);
					recorder.addNewPhase();
					recorder.setHighlightedLine(11);
					recorder.addCommand(Command::createWaitCommand());
					continue;
				}
				recorder.addNewPhase();
				recorder.setHighlightedLine(12);
				recorder.addCommand(Command(Target::Edge, Type::FadeOut, x, y));
			}
		}

		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Node, Type::HighlightOff, nodes.at(u).ui_id));
	}
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::PopUp, Type::FadeIn, -1));
}

void Graph::runKruskal(GraphRecorder& recorder) const {
	using Command = GraphAnimationCommand;
	using Type = GraphAnimationType;
	using Target = GraphAnimationTarget;
	std::vector<std::pair<int, std::pair<int, int>>> edge_list;
	for (const auto& [p, w] : edges) {
		edge_list.push_back({ w, p });
	}
	recorder.addNewPhase();
	recorder.addCommand(Command::createWaitCommand());
	recorder.setHighlightedLine(0);
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
	recorder.addNewPhase();
	recorder.addCommand(Command(Target::PopUp, Type::FadeIn, -2));
	for (const auto& [w, p] : edge_list) {
		int u = root(root, p.first);
		int v = root(root, p.second);
		int x = p.first;
		int y = p.second;
		if (x > y) std::swap(x, y);
		recorder.addNewPhase();
		recorder.addCommand(Command(Target::Edge, Type::HighlightOn, nodes.at(x).ui_id, nodes.at(y).ui_id));
		recorder.setHighlightedLine(0);
		recorder.addNewPhase();
		recorder.addCommand(Command::createWaitCommand());
		recorder.setHighlightedLine(2);
		if (join(u, v)) {
			MST += w;
			recorder.addNewPhase();
			recorder.setHighlightedLine(4);
			recorder.addCommand(Command(Target::Edge, Type::FoundedOn, nodes.at(x).ui_id, nodes.at(y).ui_id));
			recorder.addCommand(Command::createChangeValueCommand(-2, MST));
			continue;
		}
		recorder.addNewPhase();
		recorder.setHighlightedLine(3);
		recorder.addCommand(Command(Target::Edge, Type::FadeOut, nodes.at(x).ui_id, nodes.at(y).ui_id));
	}
}