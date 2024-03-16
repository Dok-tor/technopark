#include <iostream>

bool is_only_one(long long n) {
    long long bit = 0;
    bool flag = false;
    while (n) {
        bit = n & 1;
        n = n >> 1;
        if (bit) {
            if (not flag) {
                flag = true;
            } else {
                return false;
            }
        }
    }
    return flag;
}

bool test() {
    for (long long i = 0; i <= 4294967296; i++) {
        if (is_only_one(i)) {
            std::cout << i << std::endl;
        }
    }
}

int main() {
    long long n = 0;
    std::cin >> n;
    if (is_only_one(n)) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
//    test();
    return 0;
}
