//
// Created by Dok on 19.05.2024.
//

#include "SetGraph.h"
#include <cassert>

SetGraph::SetGraph(int size): adjacencyLists(size) {}

SetGraph::SetGraph(const IGraph &graph): adjacencyLists(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        auto vertexes = graph.GetNextVertices(from);
        adjacencyLists[from] = std::unordered_set(vertexes.begin(), vertexes.end());
    }
}

SetGraph::~SetGraph() = default;

void SetGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < adjacencyLists.size());
    assert(0 <= to && to < adjacencyLists.size());

    adjacencyLists[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> next;
    for (auto to : adjacencyLists[vertex])
        next.push_back(to);

    return next;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev;
    for (int from = 0; from < adjacencyLists.size(); ++from) {
        if (adjacencyLists[from].find(vertex) != adjacencyLists[from].end())
            prev.push_back(from);
    }
    return prev;
}