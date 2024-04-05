// На склад привезли много пустых ящиков. Все ящики пронумерованы по порядку поступления от 0.
// Известно, что их все можно сложить один в один (то есть так, что каждый следующий помещается в предыдущий).
// Один ящик можно вложить в другой, если его можно перевернуть так,
// что размеры одного ящика по всем осям станут строго меньше размеров другого ящика по соответствующим осям.
// Требуется определить, в какой последовательности они будут вложены друг в друга. Вывести номера ящиков.
// Для сортировки точек реализуйте алгоритм сортировки вставками.
// Максимальное кол-во 10000.

#include <iostream>

template <typename T>
int insertion_sort(T *arr, int len,  bool (*cmp) (const T&, const T&)) {
    for (int i = 1; i < len; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && cmp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
    return 0;
}

struct Box {
    int x = 0;
    int y = 0;
    int z = 0;
    int number = 0;
};

bool compare(const Box &left, const Box &right) {
    if (left.x <= right.x && left.y <= right.y && left.z <= right.z) {
        return true;
    } else if (left.x <= right.y && left.y <= right.z && left.z <= right.x) {
        return true;
    } else if (left.x <= right.z && left.y <= right.x && left.z <= right.y){
        return true;
    }
    return false;
}

int main() {
    int n = 0;
    std::cin >> n;
    Box *arr = new Box[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i].x >> arr[i].y >> arr[i].z;
        arr[i].number = i;
    }
    insertion_sort(arr, n, compare);
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i].number << " ";
    }
    delete[] arr;
    return 0;
}
