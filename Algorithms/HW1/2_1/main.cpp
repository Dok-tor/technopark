// 2_1. Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1].
// Для каждого элемента массива B[i] найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i].
// Если такого элемента нет, выведите n. n, m ≤ 10000.
// Требования:  Время работы поиска k для каждого элемента B[i]: O(log(k)).
// Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
// Формат входных данных.
// В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.

#include <iostream>

int exp_search(const int *A, int n, int value, int range[]) {
    int i = 0;
    range[0] = 1; // На случай, если нужный нам элемент находится в диапазоне индексов (1, 2)
    range[1] = n; // Если while выйдет по условию, то это будет означать,
                  // что правую границу мы так и не нашли.
    while ((1 << (i+1)) < n) {
        if (A[1 << i] < value) {
            range[0] = 1 << i;
        }
        if (value < A[1 << (i + 1)]) {
            range[1] = (1 << (i + 1)) + 1;
            break;
        }
        i++;
    }
    return 0;
}

int bin_search(const int *A, int value, const int range[]) {
    int first = range[0];
    int last = range[1];
    int mid = 0;
    while (first < last) {
        mid = (first + last) / 2;
        if (A[mid] < value) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return (A[first] >= value) ? first : -1;
}

int mixed_search(const int *A, int n, int value) {
    int result = 0;
    if (A[0] >= value) { // Проверка нулевого элемента, так как эксп. поиск начинается с 1.
        return result;
    }
    int first_last[2] = {1, 0};
    exp_search(&A[0], n, value, first_last);
    result = bin_search(&A[0], value, first_last);
    return result;
}

int search_B_in_A(const int *A, int n, const int *B, int m) {
    int result;
    for (int i = 0; i < m; i++) {
        result = mixed_search(&A[0], n, B[i]);
        std::cout << ((result != -1) ? result : n) << " ";
    }
    return 0;
}

int main() {
    int n, m;
    n = m = 0;
    std::cin >> n;
    std::cin >> m;
    int *A = new int[n];
    int *B = new int[m];
    for (int i = 0; i < n; i++) {
        std::cin >> A[i];
    }
    for (int i = 0; i < m; i++) {
        std::cin >> B[i];
    }
    search_B_in_A(&A[0], n, &B[0], m);
    delete[] A;
    delete[] B;
    return 0;
}
