#pragma once
#include <vector>
#include <string>
enum class GraphOperationType { InsertNode, RemoveNode, InsertEdge, RemoveEdge, ModifyEdge, Dijkstra, Kruskal, Reset, Save, Load, Home, Setting };

struct GraphOperation {
    GraphOperationType type;
    //Node
    int value = -1;
    //Edge
    int from = -1;
    int to = -1;
    int weight = -1;
    std::string file_path = "";

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

    static GraphOperation save(const std::string& s) {
        GraphOperation op;
        op.type = GraphOperationType::Save;
        op.file_path = s;
        return op;
    }

    static GraphOperation load(const std::string& s) {
        GraphOperation op;
        op.type = GraphOperationType::Load;
        op.file_path = s;
        return op;
    }

    static GraphOperation home() {
        GraphOperation op;
        op.type = GraphOperationType::Home;
        return op;
    }

    static GraphOperation setting() {
        GraphOperation op;
        op.type = GraphOperationType::Setting;
        return op;
    }
};