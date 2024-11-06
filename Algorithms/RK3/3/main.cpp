/*
 * Рику необходимо попасть на межвселенную конференцию. За каждую телепортацию он платит бутылками лимонада,
 * поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
 * Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день.
 * Ему известны все существующие телепортации. Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада),
 * учитывая, что телепортация не занимает времени, а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!

 * Формат ввода
 * В первой строке находятся количество вселенных N, количество существующих телепортаций M,
 * количество перелетов подряд K, номер вселенной Рика S,
 * номер вселенной конференции F (2 ≤ N ≤ 300, 1 ≤ M ≤ 105, 1 ≤ K ≤ 300, 1 ≤ S ≤ N, 1 ≤ F ≤ N).

 * Далее идет M строк, задающих телепортации. i-я строка содержит три натуральных числа:
 * Si, Fi и Pi, где Si - номер вселенной, из которой ведет i-я телепортация, Fi - номер вселенной, в которую ведет i-я телепортация,
 * Pi - стоимость i-й телепортации. 1 ≤ Si ≤ N, 1 ≤ Fi ≤ N, 1 ≤ Pi ≤ 106.

 * Формат вывода
 * Выведите одно число - минимальную стоимость пути, подходящего для профессора. Если профессор не сможет за K ночей добраться до конференции, выведите число -1.
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
//        adjacencyLists[to].emplace_back(from, weight);
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



    std::pair<int, int> Dijkstra(int from, int to) const {
        std::set<std::pair<int, int>> heap;
        std::vector<int> dist(VerticesCount(), INT_MAX);
        std::vector<int> parent(VerticesCount(), -1);

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
                    parent[next_vertex.first] = current_vertex.second;
                }
            }
        }

        if (dist[to] == INT_MAX) {
            return std::make_pair(-1, -1);
        }

        int count_jumps = 1;
        int prev = parent[to];
        while (parent[prev] != -1)
        {
            prev = parent[prev];
            count_jumps++;
        }

        auto result = std::make_pair(dist[to] == INT_MAX ? -1 : dist[to], count_jumps);

        return result;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyLists;
};

void process()
{
    int N, M, max_jumps, begin, end, from, to, cost;
    std::cin >> N >> M >> max_jumps >> begin >> end;

    ListGraph graph(N);



    for (int i = 0; i < M; ++i)
    {
        std::cin >> from >> to >> cost;
        graph.AddEdge(from - 1, to - 1, cost);
    }

    auto [whole_cost, count_jumps] = graph.Dijkstra(begin - 1, end - 1);
    std::cout << ((count_jumps <= max_jumps) and (count_jumps != -1) ? whole_cost : -1);
}

int main() {
    process();
    return 0;
}
