//
// Created by Dok on 19.05.2024.
//

#ifndef INC_1_SETGRAPH_H
#define INC_1_SETGRAPH_H

#include <unordered_set>
#include "IGraph.h"

struct SetGraph: public IGraph
{
public:
    SetGraph(int size);

    SetGraph(const IGraph &graph);

    ~SetGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> adjacencyLists;
    int vertex_count;
};

#endif //INC_1_SETGRAPH_H
