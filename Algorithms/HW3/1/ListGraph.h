//
// Created by Dok on 18.05.2024.
//

#ifndef INC_1_LISTGRAPH_H
#define INC_1_LISTGRAPH_H

#include "IGraph.h"


struct ListGraph: public IGraph
{
public:
    ListGraph(int size);

    ListGraph(const IGraph &graph);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjacencyLists;
};

#endif //INC_1_LISTGRAPH_H
