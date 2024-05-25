//
// Created by Dok on 18.05.2024.
//

#ifndef INC_1_IGRAPH_H
#define INC_1_IGRAPH_H

#include <vector>
#include <functional>
#include <deque>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func);

void mainBFS(const IGraph &graph, const std::function<void(int)> &func);

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func);

void mainDFS(const IGraph &graph, const std::function<void(int)> &func);

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted);

std::deque<int> topologicalSort(const IGraph &graph);

#endif //INC_1_IGRAPH_H
