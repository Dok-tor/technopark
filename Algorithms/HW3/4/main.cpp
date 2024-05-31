#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <set>
#include <cstring>

const char FieldSize = 16;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};



class GameState
{
public:
    GameState(const std::array<char, FieldSize> &field)
            : field(field)
    {
        emptyPos = -1;
        heuristics = 0;

        for (int i = 0; i < FieldSize; ++i)
        {
            if (field[i] == 0)
            {
                emptyPos = i;
            }
            else
            {
                heuristics += calculate_heuristics(i);
            }
        }
//        std::cout << heuristics << std::endl;
        assert(emptyPos != -1);
    }

//    int calculate_heuristics(int i)
//    {
//        if (field[i] == 0) return 0;
////        return 0;
//        int should_pos_row = (field[i] - 1) / 4;
//        int should_pos_col = (field[i] - 1) % 4;
//        int current_pos_row = i / 4;
//        int current_pos_col = i % 4;
//        return abs(current_pos_row - should_pos_row) + abs(current_pos_col - should_pos_col);
//    }

    int calculate_heuristics(int i)
    {
        if (field[i] == 0) return 0;
        int size = 4;

        int should_pos_row = (field[i] - 1) / size;
        int should_pos_col = (field[i] - 1) % size;
        int current_pos_row = i / size;
        int current_pos_col = i % size;
        int heuristic = abs(current_pos_row - should_pos_row) + abs(current_pos_col - should_pos_col);

        // Проверка линейных конфликтов в строке
        int row_start = (i / size) * size;
        int max_col = -1;
        for (int col = 0; col < size; col++)
        {
            int tile = field[row_start + col];
            if (tile != 0 && (tile - 1) / size == current_pos_row)
            {
                if (tile > max_col)
                {
                    max_col = tile;
                }
                else
                {
                    heuristic += 2;  // Линейный конфликт, добавляем штраф
                }
            }
        }

        // Проверка линейных конфликтов в столбце
        int col_start = i % size;
        int max_row = -1;
        for (int row = 0; row < size; row++)
        {
            int tile = field[row * size + col_start];
            if (tile != 0 && (tile - 1) % size == current_pos_col)
            {
                if (tile > max_row)
                {
                    max_row = tile;
                }
                else
                {
                    heuristic += 2;  // Линейный конфликт, добавляем штраф
                }
            }
        }

        return heuristic;
    }

    void update_heuristics(int old_pos, int new_pos) {
        heuristics -= calculate_heuristics(old_pos);
        heuristics += calculate_heuristics(new_pos);
    }

    bool IsComplete() const
    {
        return field == finishField;
    }

    bool IsSolvable() const
    {
        return (getInvCount() + (emptyPos / 4) + 1) % 2 == 0;
    }

    bool CanMoveLeft() const
    {
        return emptyPos % 4 != 3;
    }

    bool CanMoveRight() const
    {
        return emptyPos % 4 != 0;
    }

    bool CanMoveUp() const
    {
        return emptyPos < 12;
    }

    bool CanMoveDown() const
    {
        return emptyPos > 3;
    }

    int GetHeuristics() const
    {
        return heuristics;
    }

    GameState MoveLeft() const
    {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
//        newState.update_heuristics(emptyPos, emptyPos + 1);
        newState.calculate_heuristics();
        return newState;
    }

    GameState MoveRight() const
    {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
        newState.emptyPos--;
//        newState.update_heuristics(emptyPos, emptyPos - 1);
        newState.calculate_heuristics();
        return newState;
    }

    GameState MoveUp() const
    {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 4]);
        newState.emptyPos += 4;
//        newState.update_heuristics(emptyPos, emptyPos + 4);
        newState.calculate_heuristics();
        return newState;
    }

    GameState MoveDown() const
    {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 4]);
        newState.emptyPos -= 4;
//        newState.update_heuristics(emptyPos, emptyPos - 4);
        newState.calculate_heuristics();
        return newState;
    }

    bool operator==(const GameState &other) const
    {
        return field == other.field;
    }

    std::vector<std::pair<GameState, char>> GetNextStates()
    {
        std::vector<std::pair<GameState, char>> result;
        if (this->CanMoveLeft())
        {
            result.emplace_back(this->MoveLeft(), 'L');
        }
        if (this->CanMoveRight())
        {
            result.emplace_back(this->MoveRight(), 'R');
        }
        if (this->CanMoveDown())
        {
            result.emplace_back(this->MoveDown(), 'D');
        }
        if (this->CanMoveUp())
        {
            result.emplace_back(this->MoveUp(), 'U');
        }
        return result;
    }

private:
    void calculate_heuristics()
    {
        heuristics = 0;

        for (int i = 0; i < FieldSize; ++i)
        {
            if (field[i])
            {
                heuristics += calculate_heuristics(i);
            }
        }
//        std::cout << "heuristics " << heuristics << std::endl;
    }

    size_t getInvCount() const
    {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++)
        {
            for (int j = i + 1; j < FieldSize; j++)
            {
                if (field[i] > field[j] && field[i] && field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    std::array<char, FieldSize> field;
    char emptyPos;

    friend struct GameStateHasher;
    friend std::ostream& operator<<(std::ostream &out, const GameState &state);
    int heuristics;
};

struct GameStateHasher
{
public:
    size_t operator()(const GameState &state) const
    {
        size_t hash = 0;
        memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

std::ostream& operator<<(std::ostream &out, const GameState &state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            out << static_cast<int>(state.field[i * 4 + j]) << ' ';
        }
        out << std::endl;
    }
    return out;
}

struct VertexDistance
{
public:
    VertexDistance(GameState vertex, size_t dist): Vertex(vertex), distance(dist) {}

    bool operator<(const VertexDistance &other) const
    {
        return this->distance < other.distance;
    }
    GameState Vertex;
    size_t distance;
};

struct VertexComparator
{
    bool operator()(const VertexDistance &lhs, const VertexDistance &rhs) const
    {
        return lhs.distance < rhs.distance;
    }
};

//std::string GetSolution(const std::array<char, FieldSize> &field)
//{
//    GameState startState(field);
//
//    if (!startState.IsSolvable())
//        return "-1";
//
//    std::set<VertexDistance> heap;
//    VertexDistance start(startState, startState.GetHeuristics());
//    heap.insert(start);
//
//    std::unordered_map<GameState, size_t, GameStateHasher> dist;
//    dist[startState] = 0;
//
//    std::unordered_map<GameState, char, GameStateHasher> visited;
//    visited[startState] = 'S';
//
//    int flag = 0;
//
//    while (not heap.empty())
//    {
//        VertexDistance state = *heap.begin();
//        heap.erase(heap.begin());
//
//        if (state.Vertex.IsComplete()) {
//            flag = 1;
//            break;
//        }
//
//        std::vector<std::pair<GameState, char>> neighbors;
//
//        if (state.Vertex.CanMoveUp())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveUp(), 'U');
//        if (state.Vertex.CanMoveDown())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveDown(), 'D');
//        if (state.Vertex.CanMoveRight())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveRight(), 'R');
//        if (state.Vertex.CanMoveLeft())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveLeft(), 'L');
//
//        for (const auto& [newState, move] : neighbors)
//        {
//            size_t path = dist[state.Vertex] + 1;
//            size_t heuristic = newState.GetHeuristics();
////            std::cout << heuristic << std::endl;
//            size_t summ = path + heuristic;
//
//            auto it = dist.find(newState);
//            if (it == dist.end() || path < dist[newState])
//            {
//                visited[newState] = move;
//                dist[newState] = path;
//
//                VertexDistance neighbor(newState, summ);
//                auto iter = heap.find(neighbor);
//                if (iter == heap.end())
//                {
//                    heap.emplace(newState, summ);
//                }
//                else
//                {
//                    heap.erase(iter);
//                    heap.emplace(newState, summ);
//                }
//            }
//        }
//    }
//    std::cout << flag;
//
//    std::string path;
//    GameState state(finishField);
//
////    std::cout << state << std::endl;
//    while (visited[state] != 'S')
//    {
//        char move = visited[state];
//        switch (move)
//        {
//            case 'L':
//            {
//                state = state.MoveRight();
//                path += 'L';
//                break;
//            }
//            case 'R':
//            {
//                state = state.MoveLeft();
//                path += 'R';
//                break;
//            }
//            case 'D':
//            {
//                state = state.MoveUp();
//                path += 'D';
//                break;
//            }
//            case 'U':
//            {
//                state = state.MoveDown();
//                path += 'U';
//                break;
//            }
//        }
//
//
//
////        std::cout << state << std::endl;
//    }
//    std::reverse(path.begin(), path.end());
//    return path;
//}


std::string GetSolution(const std::array<char, FieldSize> &field)
{
    GameState start(field);

    if (!start.IsSolvable())
        return "-1";

    std::priority_queue<VertexDistance> heap;

    VertexDistance start_v(start, start.GetHeuristics());
    heap.push(start_v);

    std::unordered_map<GameState, size_t, GameStateHasher> dist;
    dist[start] = 0;

    std::unordered_map<GameState, char, GameStateHasher> visited;
    visited[start] = 'S';

    size_t initial_heuristic = start.GetHeuristics();
    double threshold = 1.5;

    while (!heap.empty())
    {
        VertexDistance current = heap.top();
        heap.pop();

        if (current.Vertex == finishField)
            break;

        for (auto [next, move] : current.Vertex.GetNextStates())
        {
            size_t new_dist = dist[current.Vertex] + 1;
            size_t heuristic = next.GetHeuristics();
//            std::cout << heuristic << std::endl;

            if (heuristic > initial_heuristic * threshold)
                continue;

            auto it = dist.find(next);
            if (it == dist.end() || new_dist < it->second)
            {
                dist[next] = new_dist;
                heap.emplace(next, new_dist + heuristic);
                visited[next] = move;
            }
        }

        threshold *= 0.9999;
    }

    if (visited.find(GameState(finishField)) == visited.end())
        return "-1";

    std::string path;
    GameState state(finishField);

    while (visited[state] != 'S')
    {
        char move = visited[state];
        switch (move)
        {
            case 'L':
                state = state.MoveRight();
                path += 'L';
                break;
            case 'R':
                state = state.MoveLeft();
                path += 'R';
                break;
            case 'D':
                state = state.MoveUp();
                path += 'D';
                break;
            case 'U':
                state = state.MoveDown();
                path += 'U';
                break;
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}



//std::string GetSolution(const std::array<char, FieldSize> &field)
//{
//    GameState startState(field);
//
//    if (!startState.IsSolvable())
//        return "-1";
//
//    std::set<VertexDistance> heap;
//    VertexDistance start(startState, startState.GetHeuristics());
//    heap.insert(start);
//
//    std::unordered_map<GameState, size_t, GameStateHasher> dist;
//    dist[startState] = 0;
//
//    std::unordered_map<GameState, char, GameStateHasher> visited;
//    visited[startState] = 'S';
//
//    int flag = 0;
//
//    while (not heap.empty())
//    {
//        VertexDistance state = *heap.begin();
//        heap.erase(heap.begin());
//        std::cout << state.Vertex << std::endl;
//
//        if (state.Vertex.IsComplete()) {
//            flag = 1;
//            break;
//        }
//
//        std::vector<std::pair<GameState, char>> neighbors;
//
//        if (state.Vertex.CanMoveUp())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveUp(), 'U');
//        if (state.Vertex.CanMoveDown())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveDown(), 'D');
//        if (state.Vertex.CanMoveRight())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveRight(), 'R');
//        if (state.Vertex.CanMoveLeft())
//            neighbors.emplace(neighbors.cend(), state.Vertex.MoveLeft(), 'L');
//
//        for (const auto& [newState, move] : neighbors)
//        {
//            size_t new_cost = dist[state.Vertex] + 1;
//            size_t heuristic = newState.GetHeuristics();
////            std::cout << heuristic << std::endl;
//
//
//            auto it = dist.find(newState);
//            if (it == dist.end() || new_cost < dist[newState])
//            {
//                visited[newState] = move;
//
//                size_t priority = new_cost + heuristic;
//                VertexDistance neighbor(newState, dist[newState]);
//                auto iter = heap.find(neighbor);
//                if (iter == heap.end())
//                {
//                    heap.emplace(newState, priority);
//                }
//                else
//                {
//                    heap.erase(iter);
//                    heap.emplace(newState, priority);
//                }
//                dist[newState] = new_cost;
//            }
//        }
//    }
////    if (not flag)
////        return "-1";
//
//    std::string path;
//    GameState state(finishField);
//
////    std::cout << state << std::endl;
//    while (visited[state] != 'S')
//    {
//        char move = visited[state];
//        switch (move)
//        {
//            case 'L':
//            {
//                state = state.MoveRight();
//                path += 'L';
//                break;
//            }
//            case 'R':
//            {
//                state = state.MoveLeft();
//                path += 'R';
//                break;
//            }
//            case 'D':
//            {
//                state = state.MoveUp();
//                path += 'D';
//                break;
//            }
//            case 'U':
//            {
//                state = state.MoveDown();
//                path += 'U';
//                break;
//            }
//        }
//
//
//
////        std::cout << state << std::endl;
//    }
//    std::reverse(path.begin(), path.end());
//    return path;
//}

void process()
{
    std::array<char, FieldSize> field{};
    field[0] = 2;
    int input;
    for (int i = 0; i < FieldSize; ++i)
    {
        std::cin >> input;
        field[i] = static_cast<char>(input);
    }

    std::string answer = GetSolution(field);
    if (answer == "-1")
    {
        std::cout << answer;
    }
    else
    {
        std::cout << answer.size() << std::endl;
        std::cout << answer;
    }
}

int main(int argc, const char * argv[]) {
//    std::array<char, FieldSize> field = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 12, 13, 14, 11, 15};
//    std::string answer = GetSolution(field);
//    std::cout << answer.size() << std::endl;
//    std::cout << answer << std::endl;
    process();
    return 0;
}
