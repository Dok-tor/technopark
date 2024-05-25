#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"
#include <iostream>

int main(int argc, const char * argv[]) {
    ListGraph listGraph(7);
    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 5);
    listGraph.AddEdge(1, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(1, 5);
    listGraph.AddEdge(1, 6);
    listGraph.AddEdge(3, 2);
    listGraph.AddEdge(3, 4);
    listGraph.AddEdge(3, 6);
    listGraph.AddEdge(5, 4);
    listGraph.AddEdge(5, 6);
    listGraph.AddEdge(6, 4);
    //----------------------------------------------------------------------------------------
    std::cout << "ListGraph" << std::endl;
    mainBFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(listGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex: topologicalSort(listGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    //----------------------------------------------------------------------------------------
    std::cout << "MatrixGraph" << std::endl;
    MatrixGraph matrixGraph(listGraph);

    mainBFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(matrixGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex: topologicalSort(matrixGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    //----------------------------------------------------------------------------------------
    std::cout << "ArcGraph" << std::endl;
    ArcGraph arcGraph(matrixGraph);

    mainBFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(arcGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex: topologicalSort(arcGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
    //----------------------------------------------------------------------------------------
    std::cout << "SetGraph" << std::endl;
    SetGraph setGraph(arcGraph);

    mainBFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    mainDFS(setGraph, [](int vertex){ std::cout << vertex << " "; });
    std::cout << std::endl;

    for (int vertex: topologicalSort(setGraph))
    {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;

    // Нужно продемонстрировать работу конструктора копирования, проинициализировав
    // графы разных классов друг от друга. Далее, показать, что вершины и ребра
    // успешно скопированы: число вершин осталось прежним, произвести BFS и DFS.
    // MatrixGraph matrixGraph(listGraph);
    // ArcGraph arcGraph(matrixGraph);
    // SetGraph setGraph(arcGraph);
    return 0;
}