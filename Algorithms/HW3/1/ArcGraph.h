//
// Created by Dok on 18.05.2024.
//

#ifndef INC_1_ARCGRAPH_H
#define INC_1_ARCGRAPH_H

#include "IGraph.h"


struct ArcGraph: public IGraph
{
public:
    ArcGraph(int size);

    ArcGraph(const IGraph &graph);

    ~ArcGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> edges_list;
    int vertex_count;
};

#endif //INC_1_ARCGRAPH_H
