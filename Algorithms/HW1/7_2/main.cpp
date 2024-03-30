// 7_2. LSD для long long.
// Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 106.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <iostream>
#include <cassert>

int get_k_byte(unsigned long long n, int k) {
    return static_cast<int>((n >> (8 * k)) & 0xFF);
}

void counting_sort(unsigned long long *arr, int n, int k) {
    assert( arr != nullptr );
    assert( n > 0 );

    int minVal = get_k_byte(arr[0], k);
    int maxVal = get_k_byte(arr[0], k);

    for (int i = 1; i < n; i++)
    {
        minVal = std::min(minVal, get_k_byte(arr[i], k));
        maxVal = std::max(maxVal, get_k_byte(arr[i], k));
    }

    int countBufSize = maxVal - minVal + 1;
    int *countBuf = new int[countBufSize]();

    for (int i = 0; i < n; i++)
    {
        countBuf[get_k_byte(arr[i], k) - minVal]++;
    }

    auto *tmpBuf = new unsigned long long[n]();

    for (int i = 1; i < countBufSize; i++)
    {
        countBuf[i] += countBuf[i - 1];
    }

    for (int i = n - 1; i >= 0; i--)
    {
        int countBufPos = get_k_byte(arr[i], k) - minVal;
        countBuf[countBufPos]--;
        tmpBuf[countBuf[countBufPos]] = arr[i];
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = tmpBuf[i];
    }

    delete[] countBuf;
    delete[] tmpBuf;
}


int LSD_sort(unsigned long long *arr, int n) {
    for (int k = 0; k < 8; ++k) {
        counting_sort(arr, n, k);
    }
    return 0;
}


int main() {
    int n;
    std::cin >> n;
    auto *arr = new unsigned long long[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    LSD_sort(arr, n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }

    delete[] arr;
    return 0;
}
