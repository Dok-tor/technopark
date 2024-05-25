//
// Created by Dok on 18.05.2024.
//

#include "MatrixGraph.h"
#include <cassert>

MatrixGraph::MatrixGraph(int size): matrix(size, std::vector<bool>(size, false)) {}

MatrixGraph::MatrixGraph(const IGraph &graph): matrix(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        for (int to: graph.GetNextVertices(from))
            matrix[from][to] = true;
    }
}

MatrixGraph::~MatrixGraph() = default;

void MatrixGraph::AddEdge(int from, int to) {
    assert(0 <= from && from < matrix.size());
    assert(0 <= to && to < matrix.size());

    // from - строка, to - столбец
    matrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(matrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> next;
    for (int to = 0; to < matrix.size(); ++to) {
        if (to != vertex and matrix[vertex][to])
            next.push_back(to);
    }
    return next;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev;
    for (int from = 0; from < matrix.size(); ++from) {
        if (from != vertex and matrix[from][vertex])
            prev.push_back(from);
    }
    return prev;
}