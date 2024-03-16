#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;
    int count = 0;
    while (n) {
        count += n & 1;
        n = n >> 2;
    }
    std::cout << count << std::endl;
    return 0;
}
