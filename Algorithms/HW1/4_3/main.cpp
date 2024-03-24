// 4.3 Планировщик процессов
// В операционной системе Technux есть планировщик процессов.
// Каждый процесс характеризуется:
// --приоритетом P
// --временем, которое он уже отработал t
// --временем, которое необходимо для завершения работы процесса T
// Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
// Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
// Требуется посчитать кол-во переключений процессора.
// Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
// Формат выходных данных: Кол-во переключений процессора.

#include <iostream>
#include <cassert>
#include <functional>

#define INIT_SIZE 5

template <typename T>
class DynamicArray {
private:
    T *arr = nullptr;
    int size;
    int capacity;

    int extend() {
        T *old_array = arr;
        arr = new T[capacity * 2];

        for (int i = 0; i < size; ++i) {
            arr[i] = old_array[i];
        }

        capacity *= 2;

        delete[] old_array;
        return 0;
    }
public:
    DynamicArray() {
        capacity = INIT_SIZE;
        arr = new T[capacity];
        size = 0;
    }
    DynamicArray(const DynamicArray &other) = delete;
    DynamicArray& operator =(const DynamicArray &other) {
        if (this != &other) {
            capacity = other.get_capacity();
            arr = new T[capacity];
            size = 0;
            for (; size < other.size; size++) {
                arr[size] = other[size];
            }
        }
        return *this;
    }
    ~DynamicArray() {
        delete[] arr;
    }

    int push_back(T value) {
        if (size == capacity) {
            extend();
        }
        arr[size++] = value;
        return 0;
    }
    T& operator[] (int index){
            return arr[index];
    }

    const T& operator[] (int index) const {
            return arr[index];
    }

    [[nodiscard]] int get_size() const {
        return size;
    }
    [[nodiscard]] int get_capacity() const {
        return capacity;
    }
    [[nodiscard]] int last() const {
        return size - 1;
    }
    int delete_last() {
        size--;
        return 0;
    }
    bool is_empty() {
        if (size == 0) {
            return true;
        }
        return false;
    }
};

template <typename T, typename Comparator>
class Heap{
private:
    Comparator cmp;
    DynamicArray<T> array;
    int size;

    int sift_down(int i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int tiniest = i;
        if (left < array.get_size() && cmp(array[left],  array[i])) {
            tiniest = left;
        }
        if (right < array.get_size() && cmp(array[right], array[tiniest])) {
            tiniest = right;
        }
        if (tiniest != i) {
            T temp = array[i];
            array[i] = array[tiniest];
            array[tiniest] = temp;
            sift_down(tiniest);
        }
        return 0;
    }

    int sift_up(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (cmp(array[parent], array[i])) {
                return 0;
            }
            T temp = array[i];
            array[i] = array[parent];
            array[parent] = temp;
            i = parent;
        }
        return 0;
    }

public:
    Heap() {
        array = DynamicArray<T>();
        size = 0;
    }
    Heap(const Heap &other) = delete;
    Heap& operator=(const Heap &other) = delete;

    int insert(T value) {
        array.push_back(value);
        sift_up(array.get_size() - 1);
        size++;
        return 0;
    }
    T extract_top() {
        assert (!array.is_empty());
        T result = array[0];
        array[0] = array[array.last()];
        array.delete_last();
        if (!array.is_empty()) {
            sift_down(0);
        }
        size--;
        return result;
    }

    bool is_empty() {
        return size == 0;
    }
};

class Process {
private:
    int P = 0;
    int t = 0;
    int T = 0;
    int priority = 0;
    int set_priority() {
        priority = P * (t + 1);
        return 0;
    }
public:

    Process()= default;
    Process(int Prior, int Time){
        P = Prior;
        T = Time;
        set_priority();
    };
    Process(const Process &other) {
        if (this != &other) {
            P = other.P;
            t = other.t;
            T = other.T;
            priority = other.priority;
        }
    }
    Process& operator =(const Process &other) {
        if (this != &other) {
            P = other.P;
            t = other.t;
            T = other.T;
            priority = other.priority;

        }
        return *this;
    }

    [[nodiscard]] int get_priority() const {
        return priority;
    }
    Process& operator++() { // Реализация увеличения t с помощью оператора инкрементирования
        t += P;
        set_priority();
        return *this;
    }
    [[nodiscard]] int get_current_time() const {
        return t;
    }
    [[nodiscard]] int get_all_time() const {
        return T;
    }
    int set_P(int value) {
        P = value;
        return 0;
    }
    int set_T(int value) {
        T = value;
        return 0;
    }
};

std::istream& operator >> (std::istream  & in, Process &process) {
    int P, T;
    P = T = 0;
    in >> P >> T;
    process.set_P(P);
    process.set_T(T);
    return in;
}

class ProcessComparator {
public:
    bool operator ()(const Process &l, const Process &r){
        if (l.get_priority() < r.get_priority()) {
            return true;
        }
        return false;
    }
};

int Technux() {
    int count = 0;
    int n = 0;
    std::cin >> n;
    Heap<Process, ProcessComparator> heap{};
    for (int i = 0; i < n; ++i) {
        Process process{};
        std::cin >> process;
        heap.insert(process);
    }

    while (!heap.is_empty()) {
        Process result = ++heap.extract_top();
        if (result.get_all_time() > result.get_current_time()) {
            heap.insert(result);
        }
        count++;
    }
    return count;
}

int main() {
    std::cout << Technux();
    return 0;
}
