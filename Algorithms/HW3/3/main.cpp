// Требуется отыскать самый короткий маршрут между городами.
// Из города может выходить дорога, которая возвращается в этот же город.
//
// Требуемое время работы O((N + M)log N), где N – количество городов, M – известных дорог между ними.
//
// Формат ввода
// Первая строка содержит число N – количество городов, 3 ≤ N ≤ 10000.
//
// Вторая строка содержит число M - количество дорог, 1 ≤ M ≤ 250000.
//
// Каждая следующая строка содержит три числа u, v, w – описание дороги: откуда, куда, время в пути.
// 0 ≤ u < N, 0 ≤ v < N, 1 ≤ w ≤ 10000. Между любыми двумя городами может быть больше одной дороги.
//
// Последняя строка содержит два числа: s и t – маршрут (откуда и куда нужно доехать). 0 ≤ s < N, 0 ≤ t < N.
//
// Формат вывода
// Вывести длину самого короткого маршрута.


#include <iostream>

#define INT_MAX 0x7fffffff

#include <vector>
#include <cassert>
#include <queue>
#include <set>

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

    void AddEdge(int from, int to, int weight) {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].emplace_back(to, weight);
        adjacencyLists[to].emplace_back(from, weight);
    }

    int VerticesCount() const {
        return static_cast<int>(adjacencyLists.size());
    }

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }

    std::vector<int> GetPrevVertices(int vertex) const {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;

        for (int from = 0; from < adjacencyLists.size(); ++from) {
            for (auto to: adjacencyLists[from]) {
                if (to.first == vertex) {
                    prevVertices.push_back(from);
                }
            }
        }
        return prevVertices;
    }



    int Dijkstra(int from, int to) const {
        std::set<std::pair<int, int>> heap;
        std::vector<int> dist(VerticesCount(), INT_MAX);

        heap.emplace(0, from);
        dist[from] = 0;

        while (not heap.empty())
        {
            // current_vertex: first - расстояние до этой вершины от нулевой, second - номер самой вершины
            std::pair<int, int> current_vertex = *heap.begin();

            heap.erase(heap.begin());
            // next_vertex: first - номер следующей вершины, second - вес ребра до неё из текущей
            for (std::pair next_vertex : GetNextVertices(current_vertex.second))
            {
                if (current_vertex.first + next_vertex.second < dist[next_vertex.first])
                {
                    // Если уже есть в куче
                    auto it = heap.find(std::make_pair(dist[next_vertex.first], next_vertex.first));
                    if (it != heap.end()) {
                        heap.erase(it);
                    }

                    heap.emplace(current_vertex.first + next_vertex.second, next_vertex.first);
                    dist[next_vertex.first] = current_vertex.first + next_vertex.second;
                }
            }
        }

        return dist[to] == INT_MAX ? -1 : dist[to];
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};


void process()
{
    int N, M;
    std::cin >> N >> M;
    ListGraph graph(N);

    int current_v, next_v, weight;
    for (int i = 0; i < M; ++i)
    {
        std::cin >> current_v >> next_v >> weight;
        graph.AddEdge(current_v, next_v, weight);
    }

    std::cin >> current_v >> next_v;

    std::cout << graph.Dijkstra(current_v, next_v);

}

int main() {
    process();
    return 0;
}
