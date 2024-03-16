#include <iostream>

int main() {
    unsigned long long n, k;
    n = k = 0;
    std::cin >> n;
    std::cin >> k;
    unsigned long long result = (n ^ (1ull << k));
    std::cout << result << std::endl;
    return 0;
}
