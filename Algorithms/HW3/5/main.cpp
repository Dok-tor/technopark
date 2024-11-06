/*
 * Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе.
 * Вариант 1. Для построения минимального остовного дерева используйте алгоритм Крускала.
 * Вариант 2. Для построения минимального остовного дерева используйте алгоритм Прима. (Мой случай)

 * Ваш номер варианта прописан в ведомости.
 * Формат ввода
 * Первая строка содержит два натуральных числа n и m — количество вершин и ребер графа соответственно (1 ≤ n ≤ 20000, 0 ≤ m ≤ 100000).
 * Следующие m строк содержат описание ребер по одному на строке.
 * Ребро номер i описывается тремя натуральными числами bi, ei и wi — номера концов ребра и его вес соответственно (1 ≤ bi, ei ≤ n, 0 ≤ wi ≤ 100000).
 *
 * Формат вывода
 * Выведите единственное целое число - вес минимального остовного дерева.
 */


#include <iostream>
#include <vector>
#include <set>
#include <cassert>

#define INT_MAX 0x7fffffff

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



    int MST() const {

        int from = 0;
        std::set<std::pair<int, int>> heap;
        std::vector<int> min_e(VerticesCount(), INT_MAX);
        std::vector<int> parent(VerticesCount(), -1);
        std::vector<bool> visited(VerticesCount(), false);

        heap.emplace(0, from);
        min_e[from] = 0;

        while (not heap.empty())
        {
            // current_vertex: first - расстояние до этой вершины от предыдущей, second - номер самой вершины
            std::pair<int, int> current_vertex = *heap.begin();

            heap.erase(heap.begin());

            visited[current_vertex.second] = true;


            // next_vertex: first - номер следующей вершины, second - вес ребра до неё из текущей
            for (std::pair next_vertex : GetNextVertices(current_vertex.second))
            {
                if (visited[next_vertex.first])
                    continue;

                if (next_vertex.second < min_e[next_vertex.first])
                {
                    // Если уже есть в куче
                    auto it = heap.find(std::make_pair(min_e[next_vertex.first], next_vertex.first));
                    if (it != heap.end()) {
                        heap.erase(it);
                    }

                    heap.emplace(next_vertex.second, next_vertex.first);
                    min_e[next_vertex.first] = next_vertex.second;
                    parent[next_vertex.first] = current_vertex.second;
                }
            }
        }

        int tree_length = 0;

        for (auto && length : min_e)
        {
            tree_length += length;
        }

        return tree_length;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};

void process()
{
    int N, M, from, to, weight;
    std::cin >> N >> M;

    ListGraph graph(N);
    for (int i = 0; i < M; ++i)
    {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from - 1, to - 1, weight);
    }

    std::cout << graph.MST();
}

int main() {
    process();
    return 0;
}
