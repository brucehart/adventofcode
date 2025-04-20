#include <iostream>
#include <cstdint>

int main() {
    std::uint64_t N;
    if (!(std::cin >> N)) {
        return 0;  // no valid input
    }

    // Find the largest power of 3 ó N
    std::uint64_t p = 1;
    while (p <= N / 3) {
        p *= 3;
    }

    std::uint64_t survivor;
    if (N == p) {
        // If N is exactly a power of 3, the survivor is N itself
        survivor = N;
    } else if (N - p <= p) {
        // If the remainder after removing p is at most p,
        // the survivor is simply that remainder
        survivor = N - p;
    } else {
        // Otherwise, the survivor wraps around further
        survivor = 2 * N - 3 * p;
    }

    std::cout << survivor << std::endl;
    return 0;
}

