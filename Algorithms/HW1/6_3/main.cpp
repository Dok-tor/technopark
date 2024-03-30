// Дано множество целых чисел из [0..10^9] размера n.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
// 10%  перцентиль
//         медиана
// 90%  перцентиль

// 6.3 Реализуйте стратегию выбора опорного элемента "случайный элемент".
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

#include <iostream>

bool int_comparator(int a, int b) {
    return a < b;
}

template <typename T, typename Comparator>
int partition(T *arr, int l, int r, Comparator cmp = Comparator()) {

    int pivot_index = rand() % (r - l + 1) + l;
    T pivot = arr[pivot_index];
    std::swap(arr[r], arr[pivot_index]);

    int i = l;
    int j = l;

    while (true) {
        if (j == r) {
            std::swap(arr[i], arr[r]);
            return i;
        }

        if (cmp(pivot, arr[j])) {
            j++;
        } else {
            std::swap(arr[i], arr[j]);
            i++;
            j++;
        }
    }
}

template <typename T, typename Comparator>
T kth_statistic(T *arr, int k, int l, int r, Comparator cmp = Comparator())
{
    while (true) {
        int pivot_pos = partition(arr, l, r, cmp);
        if (pivot_pos == k) {
            return arr[pivot_pos];
        }
        if (pivot_pos > k) {
            r = pivot_pos - 1;
        }
        else {
            l = pivot_pos + 1;
        }
    }
}

int print_statistics(int *arr, int n) {
    int k10_index = int(float(n) * 0.1);
    int mean_index = n / 2;
    int k90_index = int(float(n) * 0.9);

    int k10 = kth_statistic(arr, k10_index, 0, n - 1, int_comparator);
    std::cout << k10 << std::endl;

    int mean = kth_statistic(arr, mean_index, 0, n - 1, int_comparator);
    std::cout << mean << std::endl;

    int k90 = kth_statistic(arr, k90_index, 0, n - 1, int_comparator);
    std::cout << k90 << std::endl;

    return 0;
}

int main() {
    int n = 0;
    std::cin >> n;

    int *arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }
    print_statistics(arr, n);

    delete[] arr;
    return 0;
}
