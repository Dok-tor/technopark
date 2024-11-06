/*
 * Нужно проверить, является ли неориентированный граф эйлеровым.

   Формат ввода
   Первая строка содержит число N – количество вершин.
   Вторая строка содержит число M - количество ребер.
   Каждая следующая строка содержит ребро (откуда, куда).

   Формат вывода
   1 - если граф эйлеров
   0 - если не эйлеров
 */


#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

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

    int GetDeg(int vertex)
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return static_cast<int>(adjacencyLists[vertex].size());
    }

    int IsEuler()
    {
        std::vector<bool> visited(adjacencyLists.size(), false);
        std::queue<int> qu;
        std::vector<int> parents(adjacencyLists.size(), -1);

        qu.push(0);
        visited[0] = true;

        while (not qu.empty())
        {
            int current_vertex = qu.front();
            qu.pop();

            // Проверка степени вершины, если чётная, то всё ок, если нечётная, значит граф не Эйлеров
            if (this->GetDeg(current_vertex) % 2 != 0)
                return 0;

            for (auto next_vertex : this->GetNextVertices(current_vertex))
            {
                if (not visited[next_vertex])
                {
                    qu.push(next_vertex);
                    visited[next_vertex] = true;
                    parents[next_vertex] = current_vertex;

                }
            }
        }

        for (auto && i : visited)
        {
            if (not i)
                return 0;
        }
        return 1;
    }


private:
    std::vector<std::vector<int>> adjacencyLists;
};

void process()
{
    int N, M;
    std::cin >> N >> M;
    ListGraph graph(N);
    int from, to;
    for (int i = 0; i < M; ++i)
    {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
    }

    std::cout << graph.IsEuler();
}

int main() {
    process();
    return 0;
}
