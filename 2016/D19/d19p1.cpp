#include <iostream>
#include <cstdint>

int main() {
    std::uint64_t N;
    if (!(std::cin >> N)) {
        // No valid input
        return 0;
    }

    // Find the largest power of 2 <= N
    std::uint64_t p = 1;
    while (p <= N) {
        p <<= 1;
    }
    p >>= 1;

    // Let L be the remainder after removing that power of two
    std::uint64_t L = N - p;

    // The survivor in the "every second elf eliminated" problem
    // is 2 * L + 1
    std::uint64_t survivor = 2 * L + 1;

    std::cout << survivor << std::endl;
    return 0;
}

