//
// Created by Dok on 18.05.2024.
//

#include "ArcGraph.h"
#include <cassert>

ArcGraph::ArcGraph(int size): vertex_count(size) {}

ArcGraph::ArcGraph(const IGraph &graph): vertex_count(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from))
            edges_list.emplace_back(from, to);
    }
}

ArcGraph::~ArcGraph() = default;

void ArcGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < vertex_count);
    assert(0 <= to && to < vertex_count);
    assert(from != to);

    edges_list.emplace_back(from, to);
}

int ArcGraph::VerticesCount() const {
    return vertex_count;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> next;
    for (auto p : edges_list) {
        if (p.first == vertex)
            next.push_back(p.second);
    }
    return next;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev;
    for (auto p : edges_list) {
        if (p.second == vertex)
            prev.push_back(p.first);
    }
    return prev;
}