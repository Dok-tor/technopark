//
// Created by Dok on 18.05.2024.
//

#ifndef INC_1_MATRIXGRAPH_H
#define INC_1_MATRIXGRAPH_H

#include "IGraph.h"


struct MatrixGraph: public IGraph
{
public:
    MatrixGraph(int size);

    MatrixGraph(const IGraph &graph);

    ~MatrixGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<bool>> matrix;
};

#endif //INC_1_MATRIXGRAPH_H
