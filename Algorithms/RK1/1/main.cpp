// Дан отсортированный по возрастанию массив попарно различных целых чисел A[0..n-1].
// На вход также подается некоторое целое число k.
// Необходимо вернуть индекс элемента k в массиве A, если он там присутствует,
// либо вернуть позицию, куда его следовало бы вставить, чтобы массив остался отсортированным.
//
// Требование: сложность O(logN)

#include <iostream>

int bin_search(const int *A, int value, int n) {
    int first = 0;
    int last = n;
    int mid = 0;
    while (first < last) {
        mid = (first + last) / 2;
        if (A[mid] < value) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}

int main() {
    int n = 0;
    std::cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    int k = 0;
    std::cin >> k;
    int result = bin_search(arr, k, n);
    std::cout << result << std::endl;
    delete[] arr;
    return 0;
}
