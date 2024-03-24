// 5_4. Закраска прямой 2.
// На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка (Li и Ri).
// Найти сумму длин частей числовой прямой, окрашенных ровно в один слой.

#include <iostream>
#include <cstring>

struct Point {
    int value = 0;
    int state = 0;
};

#define INIT_SIZE 5

template <typename T>
class DynamicArray { // Этот класс скопировал из своей предыдущей задачи (4.3) и доработал для текущей
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

    int sort(bool (*cmp) (const T&, const T&)) {
        merge_sort(arr, size, cmp);
        return 0;
    }

    int merge_sort(T *array, int len,  bool (*cmp) (const T&, const T&)) {
        if (len <= 1) {
            return 0;
        }
        int first_len = len / 2;
        int second_len = len - first_len;
        merge_sort(array, first_len, cmp);
        merge_sort(array + first_len, second_len, cmp);
        T *temp = new T[len];
        merge(array, first_len, array + first_len , second_len, temp, cmp);
        memcpy(array, temp, sizeof(T) * len);
        delete[] temp;
        return 0;
    }

    int merge(T *first, int first_len, T* second, int second_len, T *recipient, bool (*cmp) (const T&, const T&)) {
        int f_i = 0;
        int s_i = 0;
        int rec_i = 0;
        while (rec_i < first_len + second_len) {
            if (f_i >= first_len) {
                recipient[rec_i++] = second[s_i++];
            } else if (s_i >= second_len) {
                recipient[rec_i++] = first[f_i++];
            } else {
                recipient[rec_i++] = cmp(first[f_i], second[s_i]) ? first[f_i++] : second[s_i++];
            }
        }
        return 0;
    }
};

bool compare_points(const Point &l, const Point &r) {
    return l.value < r.value;
}
int count_length(const DynamicArray<Point> &vector) {
    int count = 0;
    int thickness = 0;
    for (int i = 0; i < vector.get_size() - 1 ; ++i) {
        thickness += vector[i].state;
        if (thickness == 1) {
            count += vector[i+1].value - vector[i].value;
        }
    }
    return count;
}

int count_sum() {
    int n = 0;
    std::cin >> n;
    DynamicArray<Point> vector{};
    for (int i = 0; i < n; ++i) {
        Point start;
        Point end;
        std::cin >> start.value >> end.value;
        start.state = 1;
        end.state = -1;
        vector.push_back(start);
        vector.push_back(end);
    }

    vector.sort(compare_points);

    std::cout << count_length(vector) << std::endl;
    return 0;
}

int main() {
    count_sum();
    return 0;
}
