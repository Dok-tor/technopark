#include <iostream>

int main() {
    int k, n;
    k = n = 0;
    std::cin >> n;
    std::cin >> k;
    std::cout << ((n >> k) & 1) << std::endl;
    return 0;
}
