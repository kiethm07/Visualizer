#include <Model/GraphModel/GraphDictionary.h>

GraphDictionary::GraphDictionary() {
	using Type = GraphOperationType;

	pseudocode[Type::InsertNode] = {
		"If node already exists",
		"    Return",
		"Create new node with given key",
		"Done!"
	};
	titles[Type::InsertNode] = "Insert Node";

	pseudocode[Type::RemoveNode] = {
		"If node does not exist",
		"    Return",
		"For each connected neighbor",
		"    Remove edge",
		"Remove node",
		"Done!"
	};
	titles[Type::RemoveNode] = "Remove Node";

	pseudocode[Type::InsertEdge] = {
		"If edge already exists",
		"    Return",
		"Add edge (u, v, weight)",
		"Done!"
	};
	titles[Type::InsertEdge] = "Insert Edge";

	pseudocode[Type::RemoveEdge] = {
		"If edge does not exist",
		"    Return",
		"Remove edge",
		"Done!"
	};
	titles[Type::RemoveEdge] = "Remove Edge";

	pseudocode[Type::ModifyEdge] = {
		"If edge exist",
		"    Update weight",
		"Done!"
	};
	titles[Type::ModifyEdge] = "Modify Edge";

	pseudocode[Type::Dijkstra] = {
		"Init dist[x] = inf, dist[start] = 0",
		"Push {0, start} to priority queue",
		"While priority queue is not empty",
		"    Pop node u with min dist",
		"    For each neighbor v of u",
		"        If dist[u] + w < dist[v]",
		"            dist[v] = dist[u] + w",
		"            Clear parent[v] and add u",
		"            Push {dist[v], v} to pq",
		"        If dist[u] + w == dist[v]",
		"            Add u to parent[v]",
		"            Continue",
		"        Skip edge (u, v)",
		"Done!"
	};
	titles[Type::Dijkstra] = "Dijkstra's Algorithm";

	pseudocode[Type::Kruskal] = {
		"Sort edges increasingly by weight",
		"For each edge (u, v) with weight w",
		"    If u, v are in same set",
		"        Skip edge (u, v)",
		"    Add (u, v, w) to MST",
		"Done!"
	};
	titles[Type::Kruskal] = "Kruskal's Algorithm";

	pseudocode[Type::Reset] = {
		"Clear all nodes and edges",
		"Done!"
	};
	titles[Type::Reset] = "Clear Graph";
}