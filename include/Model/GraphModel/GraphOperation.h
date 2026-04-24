#pragma once
#include <vector>
enum class GraphOperationType { InsertNode, RemoveNode, InsertEdge, RemoveEdge, ModifyEdge, Dijkstra, Kruskal, Reset };

struct GraphOperation {
    GraphOperationType type;
    //Node
    int value = -1;
    //Edge
    int from = -1;
    int to = -1;
    int weight = -1;

    static GraphOperation insertNode(const int& value) {
        GraphOperation op;
        op.type = GraphOperationType::InsertNode;
        op.value = value;
        return op;
    }

    static GraphOperation removeNode(const int& value) {
        GraphOperation op;
        op.type = GraphOperationType::RemoveNode;
        op.value = value;
        return op;
    }

    static GraphOperation insertEdge(int u,int v,int w) {
        GraphOperation op;
        op.type = GraphOperationType::InsertEdge;
		op.from = u;
		op.to = v;
        op.weight = w;
        return op;
    }

    static GraphOperation removeEdge(int u,int v) {
        GraphOperation op;
        op.type = GraphOperationType::RemoveEdge;
        op.from = u;
        op.to = v;
        return op;
    }

    static GraphOperation modifyEdge(int u,int v,int w) {
        GraphOperation op;
        op.type = GraphOperationType::ModifyEdge;
        op.from = u;
        op.to = v;
        op.weight = w;
		return op;
    }

    static GraphOperation dijkstra(int start) {
        GraphOperation op;
        op.type = GraphOperationType::Dijkstra;
        op.value = start;
		return op;
    }

    static GraphOperation kruskal() {
        GraphOperation op;
        op.type = GraphOperationType::Kruskal;
        return op;
    }

    static GraphOperation reset() {
        GraphOperation op;
        op.type = GraphOperationType::Reset;
        return op;
	}
};