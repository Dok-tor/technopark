// Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
// Найдите количество различных кратчайших путей между заданными вершинами. Требуемая сложность O(V+E).
//
// Формат ввода
// v: кол-во вершин (макс. 50000),
// n: кол-во ребер(макс. 200000),
// n пар реберных вершин,
//        пара вершин (u, w) для запроса.
//
// Формат вывода
// Количество кратчайших путей от v к w.

#define INT_MAX 0x7fffffff

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

struct ListGraph
{
public:
    ListGraph(int size): adjacencyLists(size) {}

    ListGraph(const ListGraph &graph) : adjacencyLists(graph.VerticesCount()) {
        for (int i = 0; i < graph.VerticesCount(); ++i) {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }

    ~ListGraph() {}

    void AddEdge(int from, int to) {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
        adjacencyLists[to].push_back(from); // bidirect
    }

    int VerticesCount() const {
        return static_cast<int>(adjacencyLists.size());
    }

    std::vector<int> GetNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); ++from) {
            for (int to: adjacencyLists[from]) {
                if (to == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }
        return prevVertices;
    }

    [[nodiscard]] int GetCountMinWays(int from, int to) const {
        std::queue<int> qu;
        std::vector<int> dist(VerticesCount(), INT_MAX);
        std::vector<int> count_ways(VerticesCount(), 0);

        qu.push(from);
        dist[from] = 0;
        count_ways[from] = 1;

        while (not qu.empty())
        {
            int current_vertex = qu.front();
            qu.pop();
            for (int next_vertex : GetNextVertices(current_vertex))
            {


                if (dist[current_vertex] + 1 < dist[next_vertex])
                {
                    dist[next_vertex] = dist[current_vertex] + 1;
                    count_ways[next_vertex] = count_ways[current_vertex];
                    qu.push(next_vertex);
                }
                else if (dist[current_vertex] + 1 == dist[next_vertex])
                {
                    count_ways[next_vertex] += count_ways[current_vertex];
                }
            }
        }

        return count_ways[to];
    }

private:
    std::vector<std::vector<int>> adjacencyLists;
};


void process()
{
    int count_vertexes, count_edges;
    std::cin >> count_vertexes >> count_edges;
    ListGraph graph(count_vertexes);

    int from, to;

    for (int i = 0; i < count_edges; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cin >> from >> to;

    std::cout << graph.GetCountMinWays(from, to) << std::endl;
}

int main() {
    process();
    return 0;
}
